#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_lookup_table.h"
#include "bitboard/position.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"
#include "shift.h"

#include <array>
#include <type_traits>

namespace Chess
{

/// @brief Type to preallocate array for move generation
using MoveList = std::array<Bitmove, 1000>;

inline Bitmove ComposeMove(const Bitmove source,
                           const Bitmove target,
                           const Bitmove moved_piece,
                           const Bitmove captured_piece,
                           const Bitmove promotion,
                           const Bitmove move_type)
{
    return source |                                         //
           (target << MOVE_SHIFT_TARGET) |                  //
           (moved_piece << MOVE_SHIFT_MOVED_PIECE) |        //
           (captured_piece << MOVE_SHIFT_CAPTURED_PIECE) |  //
           (promotion << MOVE_SHIFT_PROMOTION) |            //
           move_type;
}

inline void PushBackAllPromotions(MoveList::iterator& move_generation_insertion_iterator,
                                  const Bitmove source,
                                  const Bitmove target,
                                  const Bitmove captured_piece)
{
    *move_generation_insertion_iterator++ =
        ComposeMove(source, target, PAWN, captured_piece, QUEEN, MOVE_VALUE_TYPE_PROMOTION);
    *move_generation_insertion_iterator++ =
        ComposeMove(source, target, PAWN, captured_piece, ROOK, MOVE_VALUE_TYPE_PROMOTION);
    *move_generation_insertion_iterator++ =
        ComposeMove(source, target, PAWN, captured_piece, KNIGHT, MOVE_VALUE_TYPE_PROMOTION);
    *move_generation_insertion_iterator++ =
        ComposeMove(source, target, PAWN, captured_piece, BISHOP, MOVE_VALUE_TYPE_PROMOTION);
}

/// @brief Type to configure behavior of GenerateMoves at compile time
///
/// This class is substituted for a mock in tests.
struct GenerateAllPseudoLegalMoves
{
    static constexpr bool generate_all_legal_moves{true};
};

/// @brief Generates all pseudo legal moves from given position
///
/// "Pseudo" in the sense that the king may be in check after generated move.
///
/// @returns An iterator pointing to the element past the last generated move
template <typename Behavior = GenerateAllPseudoLegalMoves>
std::enable_if_t<Behavior::generate_all_legal_moves, MoveList::iterator> GenerateMoves(
    const PositionWithBitboards& position,
    MoveList::iterator move_generation_insertion_iterator)
{
    const std::size_t board_idx_attacking_side = BOARD_IDX_BLACK + BOARD_IDX_BLACK_WHITE_DIFF * position.WhiteToMove();
    const std::size_t board_idx_defending_side = BOARD_IDX_BLACK_WHITE_SUM - board_idx_attacking_side;

    // pawn moves
    Bitboard pawn_board = position[board_idx_attacking_side + PAWN];
    Bitmove source_bit = tzcnt(pawn_board);
    while (source_bit < 64)
    {
        const Bitboard source = 1ULL << source_bit;
        const Bitmove pawn_capture_lookup_index_first_option =
            source_bit + PAWN_CAPUTRE_LOOKUP_TABLE_OFFSET_FOR_BLACK * !position.WhiteToMove();
        const Bitboard target_capture1 = pawn_capture_lookup_table[pawn_capture_lookup_index_first_option];
        const Bitmove target_bit_capture1 = tzcnt(target_capture1);
        const Bitboard target_capture2 = pawn_capture_lookup_table[pawn_capture_lookup_index_first_option +
                                                                   PAWN_CAPTURE_LOOKUP_TABLE_OFFSET_FOR_SECOND_OPTION];
        const Bitmove target_bit_capture2 = tzcnt(target_capture2);
        const Bitboard target_single_push = position.WhiteToMove() ? source << 8 : source >> 8;

        // captures
        if (position[board_idx_defending_side] & target_capture1)
        {
            const Bitmove captured_piece = position.GetPieceKind(board_idx_defending_side, target_capture1);
            const bool is_promotion = target_capture1 & PROMOTION_RANKS;
            if (!is_promotion)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, target_bit_capture1, PAWN, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
            }
            else
            {
                // promotion (with capture)
                PushBackAllPromotions(
                    move_generation_insertion_iterator, source_bit, tzcnt(target_capture1), captured_piece);
            }
        }
        if (position[board_idx_defending_side] & target_capture2)
        {
            const Bitmove captured_piece = position.GetPieceKind(board_idx_defending_side, target_capture2);
            const bool is_promotion = target_capture2 & PROMOTION_RANKS;
            if (!is_promotion)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, target_bit_capture2, PAWN, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
            }
            else
            {
                // promotion (with capture)
                PushBackAllPromotions(
                    move_generation_insertion_iterator, source_bit, tzcnt(target_capture2), captured_piece);
            }
        }

        // en passent
        const Bitboard en_passent_mask = (position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT);
        if (en_passent_mask)
        {
            const Bitboard en_passent_bit = en_passent_mask >> BOARD_SHIFT_EN_PASSENT;
            if (en_passent_bit == target_bit_capture1)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, target_bit_capture1, PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
            }
            if (en_passent_bit == target_bit_capture2)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, target_bit_capture2, PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
            }
        }

        // single push
        const bool target_single_push_is_occupied =
            (position[BOARD_IDX_BLACK] | position[BOARD_IDX_WHITE]) & target_single_push;
        if (!target_single_push_is_occupied)
        {
            const bool is_promotion = target_single_push & PROMOTION_RANKS;
            if (!is_promotion)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, tzcnt(target_single_push), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH);
            }
            else
            {
                // promotion (without capture)
                PushBackAllPromotions(
                    move_generation_insertion_iterator, source_bit, tzcnt(target_single_push), NO_CAPTURE);
            }
        }

        // double push
        const bool source_is_on_start_row = (position.WhiteToMove() && (source & START_RANK_WHITE)) ||
                                            (!position.WhiteToMove() && (source & START_RANK_BLACK));
        if (source_is_on_start_row)
        {
            const Bitboard target_double_push = position.WhiteToMove() ? source << 16 : source >> 16;
            const bool target_double_push_is_occupied = (position[board_idx_attacking_side] & target_double_push) ||
                                                        (position[board_idx_defending_side] & target_double_push);
            if (!target_single_push_is_occupied && !target_double_push_is_occupied)
            {
                *move_generation_insertion_iterator++ = ComposeMove(source_bit,
                                                                    tzcnt(target_double_push),
                                                                    PAWN,
                                                                    NO_CAPTURE,
                                                                    NO_PROMOTION,
                                                                    MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH);
            }
        }

        // prepare next iteration
        pawn_board &= ~source;
        source_bit = tzcnt(pawn_board);
    }

    // knight moves

    // bishop moves

    // rook moves

    // queen moves

    {  // king moves
        const Bitboard king_board = position[board_idx_attacking_side + KING];
        constexpr std::array<std::size_t, 8> king_directions{
            west, north_west, north, north_east, east, south_east, south, south_west};
        for (const auto direction : king_directions)
        {
            const Bitboard target = Shift(king_board, direction);
            if (target)  // is on board?
            {
                const Bitmove source_bit = tzcnt(king_board);
                const bool target_is_free =
                    target & ~(position[board_idx_attacking_side] | position[board_idx_defending_side]);
                if (target_is_free)
                {
                    *move_generation_insertion_iterator++ = ComposeMove(
                        source_bit, tzcnt(target), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN);
                    continue;
                }
                const bool target_is_occupied_by_opponents_piece = target & position[board_idx_defending_side];
                if (target_is_occupied_by_opponents_piece)
                {
                    const Bitmove captured_piece = position.GetPieceKind(board_idx_defending_side, target);
                    *move_generation_insertion_iterator++ = ComposeMove(
                        source_bit, tzcnt(target), KING, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
                }
            }
        }
    }

    return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
