#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_lookup_table.h"
#include "bitboard/position.h"
#include "bitboard/squares.h"
#include "bitboard/shift.h"
#include "bitboard/move_list.h"
#include "hardware/trailing_zeros_count.h"

#include <array>
#include <type_traits>
#include <functional>

namespace Chess
{

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
    /// @brief A function to loop over individual bits (the population) of a Bitboard
    const auto ForEveryBitInPopulation =
        [](const Bitboard population,
           const std::function<void(const Bitmove source_bit, const Bitboard source)> loop_body) {
            // prepare entry condition
            Bitboard remaining_population = population;
            Bitmove current_source_bit = tzcnt(remaining_population);
            while (current_source_bit < 64)
            {
                const Bitboard current_source = 1ULL << current_source_bit;

                // execute body
                loop_body(current_source_bit, current_source);

                // prepare next iteration
                remaining_population &= ~current_source;
                current_source_bit = tzcnt(remaining_population);
            }
        };

    const auto PushBackAllPromotions = [](MoveList::iterator& move_generation_insertion_iterator,
                                          const Bitmove source,
                                          const Bitmove target,
                                          const Bitmove captured_piece) {
        *move_generation_insertion_iterator++ =
            ComposeMove(source, target, PAWN, captured_piece, QUEEN, MOVE_VALUE_TYPE_PROMOTION);
        *move_generation_insertion_iterator++ =
            ComposeMove(source, target, PAWN, captured_piece, ROOK, MOVE_VALUE_TYPE_PROMOTION);
        *move_generation_insertion_iterator++ =
            ComposeMove(source, target, PAWN, captured_piece, KNIGHT, MOVE_VALUE_TYPE_PROMOTION);
        *move_generation_insertion_iterator++ =
            ComposeMove(source, target, PAWN, captured_piece, BISHOP, MOVE_VALUE_TYPE_PROMOTION);
    };

    const bool white_to_move = position.WhiteToMove();
    const std::size_t attacking_side = BOARD_IDX_BLACK + BOARD_IDX_BLACK_WHITE_DIFF * white_to_move;
    const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
    const Bitboard free_squares = ~(position[BOARD_IDX_BLACK] | position[BOARD_IDX_WHITE]);

    // pawn moves
    const auto generate_pawn_move = [&](const Bitmove source_bit, const Bitboard source) {
        const Bitmove pawn_capture_lookup_index_first_option =
            source_bit + PAWN_CAPUTRE_LOOKUP_TABLE_OFFSET_FOR_BLACK * !white_to_move;
        const std::array<Bitboard, 2> pawn_capture_targets{
            pawn_capture_lookup_table[pawn_capture_lookup_index_first_option],
            pawn_capture_lookup_table[pawn_capture_lookup_index_first_option +
                                      PAWN_CAPTURE_LOOKUP_TABLE_OFFSET_FOR_SECOND_OPTION]};
        const std::array<Bitboard, 2> pawn_capture_target_bits{tzcnt(std::get<0>(pawn_capture_targets)),
                                                               tzcnt(std::get<1>(pawn_capture_targets))};
        const Bitboard target_single_push = white_to_move ? source << 8 : source >> 8;

        // captures
        for (const std::size_t index : {0, 1})
        {
            if (position[defending_side] & pawn_capture_targets.at(index))
            {
                const Bitmove captured_piece = position.GetPieceKind(defending_side, pawn_capture_targets.at(index));
                const bool is_promotion = pawn_capture_targets.at(index) & PROMOTION_RANKS;
                if (!is_promotion)
                {
                    *move_generation_insertion_iterator++ = ComposeMove(source_bit,
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
                                          source_bit,
                                          tzcnt(pawn_capture_targets.at(index)),
                                          captured_piece);
                }
            }
        }

        // en passant
        const Bitboard en_passant_mask = position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT;
        if (en_passant_mask)
        {
            const Bitboard en_passant_bit = en_passant_mask >> BOARD_SHIFT_EN_PASSANT;
            for (const std::size_t index : {0, 1})
            {
                if (en_passant_bit == pawn_capture_target_bits.at(index))
                {
                    *move_generation_insertion_iterator++ = ComposeMove(source_bit,
                                                                        pawn_capture_target_bits.at(index),
                                                                        PAWN,
                                                                        PAWN,
                                                                        NO_PROMOTION,
                                                                        MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE);
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
        const bool source_is_on_start_row =
            (white_to_move && (source & START_RANK_WHITE)) || (!white_to_move && (source & START_RANK_BLACK));
        if (source_is_on_start_row)
        {
            const Bitboard target_double_push = white_to_move ? source << 16 : source >> 16;
            const bool target_double_push_is_free = target_double_push & free_squares;
            if (target_single_push_is_free && target_double_push_is_free)
            {
                *move_generation_insertion_iterator++ = ComposeMove(source_bit,
                                                                    tzcnt(target_double_push),
                                                                    PAWN,
                                                                    NO_CAPTURE,
                                                                    NO_PROMOTION,
                                                                    MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH);
            }
        }
    };
    ForEveryBitInPopulation(position[attacking_side + PAWN], generate_pawn_move);

    /// @brief Ray in the sense that all squares in a certain direction are considered as targets
    const auto generate_ray_style_move = [&](const std::size_t direction,
                                             const Bitmove source_bit,
                                             const Bitboard source,
                                             const std::size_t moved_piece) {
        Bitboard target = SingleStep(source, direction);
        while (target)  // is on the board
        {
            const bool target_is_blocked_by_own_piece = target & position[attacking_side];
            if (target_is_blocked_by_own_piece)
            {
                break;
            }

            const bool target_is_free = target & free_squares;
            if (target_is_free)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, tzcnt(target), moved_piece, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN);
            }
            else  // target occupied by opposing piece
            {
                const Bitmove captured_piece = position.GetPieceKind(defending_side, target);
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, tzcnt(target), moved_piece, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
                break;
            }
            target = SingleStep(target, direction);
        }
    };

    // bishop moves
    const auto generate_bishop_move = [&](const Bitmove source_bit, const Bitboard source) {
        for (const auto direction : bishop_directions)
        {
            generate_ray_style_move(direction, source_bit, source, BISHOP);
        }
    };
    ForEveryBitInPopulation(position[attacking_side + BISHOP], generate_bishop_move);

    // rook moves
    const auto generate_rook_move = [&](const Bitmove source_bit, const Bitboard source) {
        for (const auto direction : rook_directions)
        {
            generate_ray_style_move(direction, source_bit, source, ROOK);
        }
    };
    ForEveryBitInPopulation(position[attacking_side + ROOK], generate_rook_move);

    // queen moves
    const auto generate_queen_move = [&](const Bitmove source_bit, const Bitboard source) {
        for (const auto direction : all_directions)
        {
            generate_ray_style_move(direction, source_bit, source, QUEEN);
        }
    };
    ForEveryBitInPopulation(position[attacking_side + QUEEN], generate_queen_move);

    /// @brief Jump in the sense that only target and source are considered (possible in between squares are ignored)
    const auto generate_jump_style_move = [&](const Bitboard source,
                                              const Bitboard target,
                                              const std::size_t moved_piece) {
        if (target)  // is on the board?
        {
            const Bitmove source_bit = tzcnt(source);
            const bool target_is_free = target & free_squares;
            if (target_is_free)
            {
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, tzcnt(target), moved_piece, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN);
                return;
            }
            const bool target_is_occupied_by_opponents_piece = target & position[defending_side];
            if (target_is_occupied_by_opponents_piece)
            {
                const Bitmove captured_piece = position.GetPieceKind(defending_side, target);
                *move_generation_insertion_iterator++ = ComposeMove(
                    source_bit, tzcnt(target), moved_piece, captured_piece, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE);
            }
        }
    };

    // knight moves
    const auto generate_knight_move = [&](const Bitmove /*unused*/, const Bitboard source) {
        for (const auto jump_direction : knight_jumps)
        {
            const Bitboard target = KnightJump(source, jump_direction);
            generate_jump_style_move(source, target, KNIGHT);
        }
    };
    ForEveryBitInPopulation(position[attacking_side + KNIGHT], generate_knight_move);

    // king moves
    const Bitboard king_board = position[attacking_side + KING];
    for (const auto direction : all_directions)
    {
        const Bitboard target = SingleStep(king_board, direction);
        generate_jump_style_move(king_board, target, KING);
    }

    // castling
    constexpr std::array<Bitboard, 4> castling_rights{BOARD_VALUE_CASTLING_BLACK_KINGSIDE,
                                                      BOARD_VALUE_CASTLING_BLACK_QUEENSIDE,
                                                      BOARD_VALUE_CASTLING_WHITE_KINGSIDE,
                                                      BOARD_VALUE_CASTLING_WHITE_QUEENSIDE};
    constexpr std::array<Bitboard, 4> neccessary_free_squares = {F8 | G8, D8 | C8 | B8, F1 | G1, D1 | C1 | B1};
    constexpr std::array<int, 4> source_bits = {59, 59, 3, 3};
    constexpr std::array<int, 4> target_bits{57, 61, 1, 5};
    constexpr std::array<Bitmove, 4> move_values{MOVE_VALUE_TYPE_KINGSIDE_CASTLING,
                                                 MOVE_VALUE_TYPE_QUEENSIDE_CASTLING,
                                                 MOVE_VALUE_TYPE_KINGSIDE_CASTLING,
                                                 MOVE_VALUE_TYPE_QUEENSIDE_CASTLING};

    const std ::size_t offset_for_white = 2 * white_to_move;
    for (std::size_t side = 0; side < 2; side++)  // side as in queen- or kingside, not white or black
    {
        const std::size_t castling = side + offset_for_white;
        const bool castling_to_side_is_allowed = position[BOARD_IDX_EXTRAS] & castling_rights[castling];
        const bool space_between_king_and_rook_is_free =
            (free_squares & neccessary_free_squares[castling]) == neccessary_free_squares[castling];
        const bool castling_possible = castling_to_side_is_allowed && space_between_king_and_rook_is_free;
        if (castling_possible)
        {
            *move_generation_insertion_iterator++ = ComposeMove(
                source_bits[castling], target_bits[castling], KING, NO_CAPTURE, NO_PROMOTION, move_values[castling]);
        }
    }

    return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
