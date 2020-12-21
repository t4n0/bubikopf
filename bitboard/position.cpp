#include "bitboard/position.h"

#include "bitboard/lookup_table/bishop.h"
#include "bitboard/lookup_table/king.h"
#include "bitboard/lookup_table/knight.h"
#include "bitboard/lookup_table/rook.h"
#include "bitboard/pieces.h"
#include "bitboard/shift.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <stdexcept>

namespace Chess
{

Bitmove Position::GetPieceKind(const std::size_t side, const Bitboard location) const
{
    if (boards_[side + PAWN] & location)
    {
        return PAWN;
    }
    if (boards_[side + KNIGHT] & location)
    {
        return KNIGHT;
    }
    if (boards_[side + BISHOP] & location)
    {
        return BISHOP;
    }
    if (boards_[side + ROOK] & location)
    {
        return ROOK;
    }
    if (boards_[side + QUEEN] & location)
    {
        return QUEEN;
    }
    if (boards_[side + KING] & location)
    {
        return KING;
    }
    throw std::runtime_error{"Querry for piece kind failed."};
}

void Position::MakeMove(Bitmove move)
{
    *extras_history_insertion_index_++ = boards_[BOARD_IDX_EXTRAS];
    const Bitboard& current_extras =
        *(extras_history_insertion_index_ - 1);  // Moving side, en-passant etc. will be changed in
                                                 // position. Hence getting unaltered state from history.

    const std::size_t attacking_piece = (move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE;
    const std::size_t attacking_piece_index = attacking_side_ + attacking_piece;

    const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
    const Bitboard target = BOARD_ONE << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
    const Bitboard source_and_target = source | target;

    boards_[BOARD_IDX_EXTRAS] &= ~(BOARD_MASK_EN_PASSANT | BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE |
                                   BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE);

    boards_[attacking_side_] ^= source_and_target;
    boards_[attacking_piece_index] ^= source_and_target;

    const Bitmove move_type = move & MOVE_MASK_TYPE;
    switch (move_type)
    {
        case MOVE_VALUE_TYPE_QUIET_NON_PAWN:
        {
            boards_[BOARD_IDX_EXTRAS]++;
            break;
        }
        case MOVE_VALUE_TYPE_CAPTURE:
        {
            const std::size_t captured_piece =
                defending_side_ + ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
            boards_[defending_side_] &= ~target;
            boards_[captured_piece] &= ~target;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            break;
        }
        case MOVE_VALUE_TYPE_PAWN_PUSH:
        {
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            break;
        }
        case MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH:
        {
            const Bitmove source_bit = move & MOVE_MASK_SOURCE;
            const Bitmove target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
            boards_[BOARD_IDX_EXTRAS] |= ((source_bit + target_bit) >> 1)
                                         << BOARD_SHIFT_EN_PASSANT;  // (source_bit+target_bit)/2
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            break;
        }
        case MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE:
        {
            const Bitboard en_passant_square = BOARD_ONE
                                               << ((current_extras & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT);
            const Bitboard en_passant_victim = white_to_move_ ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[defending_side_] &= ~en_passant_victim;
            boards_[defending_side_ + PAWN] &= ~en_passant_victim;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            break;
        }
        case MOVE_VALUE_TYPE_KINGSIDE_CASTLING:
        {
            const Bitboard source_rook = target >> 1;
            const Bitboard target_rook = target << 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_side_] ^= source_and_target_rook_jump;
            boards_[attacking_side_ + ROOK] ^= source_and_target_rook_jump;
            boards_[BOARD_IDX_EXTRAS]++;
            boards_[BOARD_IDX_EXTRAS] |= BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE;
            break;
        }
        case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING:
        {
            const Bitboard source_rook = target << 2;
            const Bitboard target_rook = target >> 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_side_] ^= source_and_target_rook_jump;
            boards_[attacking_side_ + ROOK] ^= source_and_target_rook_jump;
            boards_[BOARD_IDX_EXTRAS]++;
            boards_[BOARD_IDX_EXTRAS] |= BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE;
            break;
        }
        case MOVE_VALUE_TYPE_PROMOTION:
        {
            boards_[attacking_piece_index] ^= source_and_target;  // revert default operation before switch
            const std::size_t board_idx_added_piece_kind =
                attacking_side_ + ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
            boards_[attacking_piece_index] &= ~source;
            boards_[board_idx_added_piece_kind] |= target;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
            if (capture)
            {
                const std::size_t captured_piece = defending_side_ + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
                boards_[defending_side_] &= ~target;
                boards_[captured_piece] &= ~target;
            }
            break;
        }
    }

    // revoke castling rights
    const bool someone_can_still_castle = current_extras & BOARD_MASK_CASTLING;
    if (someone_can_still_castle)
    {
        constexpr Bitboard white_queenside_castling_squares = A1 | E1;
        if (source_and_target & white_queenside_castling_squares)
        {
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_QUEENSIDE;
        }
        constexpr Bitboard white_kingside_castling_squares = E1 | H1;
        if (source_and_target & white_kingside_castling_squares)
        {
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
        }
        constexpr Bitboard black_queenside_castling_squares = A8 | E8;
        if (source_and_target & black_queenside_castling_squares)
        {
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
        }
        constexpr Bitboard black_kingside_castling_squares = E8 | H8;
        if (source_and_target & black_kingside_castling_squares)
        {
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
        }
    }

    white_to_move_ = !white_to_move_;
    attacking_side_ ^= BOARD_IDX_TOGGLE_SIDE;
    defending_side_ ^= BOARD_IDX_TOGGLE_SIDE;
}

void Position::UnmakeMove(Bitmove move)
{
    extras_history_insertion_index_--;
    boards_[BOARD_IDX_EXTRAS] = *extras_history_insertion_index_;
    white_to_move_ = !white_to_move_;
    attacking_side_ ^= BOARD_IDX_TOGGLE_SIDE;
    defending_side_ ^= BOARD_IDX_TOGGLE_SIDE;

    const std::size_t attacking_piece_index =
        attacking_side_ + ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE);

    const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
    const Bitboard target = BOARD_ONE << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
    const Bitboard source_and_target = source | target;

    boards_[attacking_side_] ^= source_and_target;
    boards_[attacking_piece_index] ^= source_and_target;

    const Bitmove move_type = move & MOVE_MASK_TYPE;
    switch (move_type)
    {
        case MOVE_VALUE_TYPE_CAPTURE:
        {
            const std::size_t captured_piece =
                defending_side_ + ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
            boards_[defending_side_] |= target;
            boards_[captured_piece] |= target;
            return;
        }

        case MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE:
        {
            const Bitboard en_passant_square =
                BOARD_ONE << ((boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT);
            const Bitboard en_passant_victim = white_to_move_ ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[defending_side_] |= en_passant_victim;
            boards_[defending_side_ + PAWN] |= en_passant_victim;
            return;
        }
        case MOVE_VALUE_TYPE_KINGSIDE_CASTLING:
        {
            const Bitboard source_rook = target >> 1;
            const Bitboard target_rook = target << 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_side_] ^= source_and_target_rook_jump;
            boards_[attacking_side_ + ROOK] ^= source_and_target_rook_jump;
            return;
        }
        case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING:
        {
            const Bitboard source_rook = target << 2;
            const Bitboard target_rook = target >> 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_side_] ^= source_and_target_rook_jump;
            boards_[attacking_side_ + ROOK] ^= source_and_target_rook_jump;
            return;
        }
        case MOVE_VALUE_TYPE_PROMOTION:
        {
            boards_[attacking_piece_index] ^= source_and_target;  // revert default operation before switch
            const std::size_t board_idx_added_piece_kind =
                attacking_side_ + ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
            boards_[attacking_piece_index] |= source;
            boards_[board_idx_added_piece_kind] &= ~target;
            const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
            if (capture)
            {
                const std::size_t captured_piece = defending_side_ + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
                boards_[defending_side_] |= target;
                boards_[captured_piece] |= target;
            }
            return;
        }
    }
}

Bitboard& Position::operator[](const std::size_t index)
{
    return boards_[index];
}

Bitboard Position::operator[](const std::size_t index) const
{
    return boards_[index];
}

bool operator==(const Position& a, const Position& b)
{
    const bool boards_are_equal = a.boards_ == b.boards_;
    const bool playing_side_is_equal = a.white_to_move_ == b.white_to_move_;
    return boards_are_equal && playing_side_is_equal;
}

bool Position::DefendersKingIsInCheck() const
{
    const auto ray_check_given =
        [&](const Bitboard square, const std::size_t direction, const std::size_t dangerous_piece_besides_queen) {
            Bitboard attacker_location = SingleStep(square, direction);
            while (attacker_location)  // is on the board
            {
                const bool location_is_occupied_by_friendly_piece = attacker_location & boards_[defending_side_];
                if (location_is_occupied_by_friendly_piece)
                {
                    break;  // this direction is safe
                }

                const bool location_is_occupied_by_enemy_piece = attacker_location & boards_[attacking_side_];
                if (location_is_occupied_by_enemy_piece)
                {
                    const std::size_t piece_kind = GetPieceKind(attacking_side_, attacker_location);
                    const bool piece_can_attack_from_this_angle =
                        (piece_kind == QUEEN) || (piece_kind == dangerous_piece_besides_queen);
                    if (piece_can_attack_from_this_angle)
                    {
                        return true;
                    }
                    else
                    {
                        break;  // this direction is safe
                    }
                }

                attacker_location = SingleStep(attacker_location, direction);  // next iteration
            }

            return false;
        };

    const auto square_is_under_attack = [&](const Bitboard square) {
        const int square_bit = tzcnt(square);

        // ray style checks horizontal and vertical
        const bool rook_or_queen_aligend_on_rank_or_file =
            rook_attacks[square_bit] & (boards_[attacking_side_ + ROOK] | boards_[attacking_side_ + QUEEN]);
        if (rook_or_queen_aligend_on_rank_or_file)
        {
            for (const auto direction : {west, south, east, north})
            {
                if (ray_check_given(square, direction, ROOK))
                {
                    return true;
                }
            }
        }

        // ray style checks diagonally
        const bool bishop_or_queen_aligend_diagonally =
            bishop_attacks[square_bit] & (boards_[attacking_side_ + BISHOP] | boards_[attacking_side_ + QUEEN]);
        if (bishop_or_queen_aligend_diagonally)
        {
            for (const auto direction : {north_east, north_west, south_east, south_west})
            {
                if (ray_check_given(square, direction, BISHOP))
                {
                    return true;
                }
            }
        }

        // knight checks
        const bool knight_is_giving_check = knight_jumps[square_bit] & boards_[attacking_side_ + KNIGHT];
        if (knight_is_giving_check)
        {
            return true;
        }

        // pawn checks
        const bool pawn_is_giving_check = pawn_attacks[square_bit + (white_to_move_ * pawn_attacks_offset_for_white)] &
                                          boards_[attacking_side_ + PAWN];
        if (pawn_is_giving_check)
        {
            return true;
        }

        // king checks
        const bool king_is_giving_check = king_attacks[square_bit] & boards_[attacking_side_ + KING];
        if (king_is_giving_check)
        {
            return true;
        }

        return false;
    };

    const Bitboard king_location = boards_[defending_side_ + KING];
    if (square_is_under_attack(king_location))
    {
        return true;
    }

    const bool defending_side_just_castled = boards_[BOARD_IDX_EXTRAS] & (BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE |
                                                                          BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE);
    if (defending_side_just_castled)
    {
        // pass through square
        const bool defending_side_just_castled_kingside =
            boards_[BOARD_IDX_EXTRAS] & BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE;
        const Bitboard pass_through_square_of_king =
            defending_side_just_castled_kingside ? king_location << 1 : king_location >> 1;
        if (square_is_under_attack(pass_through_square_of_king))
        {
            return true;
        }

        // previous square
        const Bitboard previous_square_of_king =
            defending_side_just_castled_kingside ? king_location << 2 : king_location >> 2;
        if (square_is_under_attack(previous_square_of_king))
        {
            return true;
        }
    }

    return false;
}

}  // namespace Chess
