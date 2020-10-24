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

/// TODO: Throw when trying to assign past the end.
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
    const Bitboard free_squares = ~(position[BOARD_IDX_BLACK] | position[BOARD_IDX_WHITE]);

    // pawn moves
    Bitboard pawn_board = position[board_idx_attacking_side + PAWN];
    Bitmove pawn_source_bit = tzcnt(pawn_board);
    while (pawn_source_bit < 64)
    {
        const Bitboard source = 1ULL << pawn_source_bit;
        const Bitmove pawn_capture_lookup_index_first_option =
            pawn_source_bit + PAWN_CAPUTRE_LOOKUP_TABLE_OFFSET_FOR_BLACK * !position.WhiteToMove();
        const std::array<Bitboard, 2> pawn_capture_targets{
            pawn_capture_lookup_table[pawn_capture_lookup_index_first_option],
            pawn_capture_lookup_table[pawn_capture_lookup_index_first_option +
                                      PAWN_CAPTURE_LOOKUP_TABLE_OFFSET_FOR_SECOND_OPTION]};
        const std::array<Bitboard, 2> pawn_capture_target_bits{tzcnt(std::get<0>(pawn_capture_targets)),
                                                               tzcnt(std::get<1>(pawn_capture_targets))};
        const Bitboard target_single_push = position.WhiteToMove() ? source << 8 : source >> 8;

        // captures
        for (const std::size_t index : {0, 1})
        {
            if (position[board_idx_defending_side] & pawn_capture_targets.at(index))
            {
                const Bitmove captured_piece =
                    position.GetPieceKind(board_idx_defending_side, pawn_capture_targets.at(index));
                const bool is_promotion = pawn_capture_targets.at(index) & PROMOTION_RANKS;
                if (!is_promotion)
                {
                    *move_generation_insertion_iterator++ = ComposeMove(pawn_source_bit,
                                                                        pawn_capture_target_bits.at(index),
                                                                        PAWN,
                                                                        captured_piece,
                                                                        NO_PROMOTION,
                                                                        MOVE_VALUE_TYPE_CAPTURE);
                }
                else
                {
                    // promotion (with capture)
                    PushBackAllPromotions(move_generation_insertion_iterator,
                                          pawn_source_bit,
                                          tzcnt(pawn_capture_targets.at(index)),
                                          captured_piece);
                }
            }
        }

        // en passent
        const Bitboard en_passent_mask = position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT;
        if (en_passent_mask)
        {
            const Bitboard en_passent_bit = en_passent_mask >> BOARD_SHIFT_EN_PASSENT;
            for (const std::size_t index : {0, 1})
            {
                if (en_passent_bit == pawn_capture_target_bits.at(index))
                {
                    *move_generation_insertion_iterator++ = ComposeMove(pawn_source_bit,
                                                                        pawn_capture_target_bits.at(index),
                                                                        PAWN,
                                                                        PAWN,
                                                                        NO_PROMOTION,
                                                                        MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
                }
            }
        }

        // single push
        const bool target_single_push_is_free = target_single_push & free_squares;
        if (target_single_push_is_free)
        {
            const bool is_promotion = target_single_push & PROMOTION_RANKS;
            if (!is_promotion)
            {
                *move_generation_insertion_iterator++ = ComposeMove(pawn_source_bit,
                                                                    tzcnt(target_single_push),
                                                                    PAWN,
                                                                    NO_CAPTURE,
                                                                    NO_PROMOTION,
                                                                    MOVE_VALUE_TYPE_PAWN_PUSH);
            }
            else
            {
                // promotion (without capture)
                PushBackAllPromotions(
                    move_generation_insertion_iterator, pawn_source_bit, tzcnt(target_single_push), NO_CAPTURE);
            }
        }

        // double push
        const bool source_is_on_start_row = (position.WhiteToMove() && (source & START_RANK_WHITE)) ||
                                            (!position.WhiteToMove() && (source & START_RANK_BLACK));
        if (source_is_on_start_row)
        {
            const Bitboard target_double_push = position.WhiteToMove() ? source << 16 : source >> 16;
            const bool target_double_push_is_free = target_double_push & free_squares;
            if (target_single_push_is_free && target_double_push_is_free)
            {
                *move_generation_insertion_iterator++ = ComposeMove(pawn_source_bit,
                                                                    tzcnt(target_double_push),
                                                                    PAWN,
                                                                    NO_CAPTURE,
                                                                    NO_PROMOTION,
                                                                    MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH);
            }
        }

        // prepare next iteration
        pawn_board &= ~source;
        pawn_source_bit = tzcnt(pawn_board);
    }

    // knight moves

    // bishop moves

    // rook moves
    Bitboard rook_board = position[board_idx_attacking_side + ROOK];
    Bitmove rook_source_bit = tzcnt(rook_board);
    Bitboard rook_source = 1ULL << rook_source_bit;
    while (rook_source_bit < 64)
    {
        constexpr std::array<std::size_t, 4> rook_directions{west, north, east, south};
        for (const auto direction : rook_directions)
        {
            Bitboard target = Shift(rook_source, direction);
            while (target)  // is on board
            {
                const bool target_is_blocked_by_own_piece = target & position[board_idx_attacking_side];
                if (target_is_blocked_by_own_piece)
                {
                    break;
                }

                const bool target_is_free = target & free_squares;
                if (target_is_free)
                {
                    *move_generation_insertion_iterator++ = ComposeMove(
                        rook_source_bit, tzcnt(target), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN);
                }
                else  // target blocked by opposing piece
                {
                    const Bitmove captured_piece = position.GetPieceKind(board_idx_defending_side, target);
                    *move_generation_insertion_iterator++ = ComposeMove(
                        rook_source_bit, tzcnt(target), ROOK, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
                }
                target = Shift(target, direction);
            }
        }

        // prepare next iteration
        rook_board &= ~rook_source;
        rook_source_bit = tzcnt(rook_board);
        rook_source = 1ULL << rook_source_bit;
    }

    // queen moves

    // king moves
    const Bitboard king_board = position[board_idx_attacking_side + KING];
    constexpr std::array<std::size_t, 8> king_directions{
        west, north_west, north, north_east, east, south_east, south, south_west};
    for (const auto direction : king_directions)
    {
        const Bitboard target = Shift(king_board, direction);
        if (target)  // is on the board?
        {
            const Bitmove king_source_bit = tzcnt(king_board);
            const bool target_is_free = target & free_squares;
            if (target_is_free)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    king_source_bit, tzcnt(target), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN);
                continue;
            }
            const bool target_is_occupied_by_opponents_piece = target & position[board_idx_defending_side];
            if (target_is_occupied_by_opponents_piece)
            {
                const Bitmove captured_piece = position.GetPieceKind(board_idx_defending_side, target);
                *move_generation_insertion_iterator++ = ComposeMove(
                    king_source_bit, tzcnt(target), KING, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
            }
        }
    }

    return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
