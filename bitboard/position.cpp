#include "bitboard/position.h"

#include "bitboard/lookup_table/knight.h"
#include "bitboard/pieces.h"
#include "bitboard/shift.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <stdexcept>

namespace Chess
{

bool PositionWithBitboards::WhiteToMove() const
{
    return boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN;
}

Bitmove PositionWithBitboards::GetPieceKind(const std::size_t side, const Bitboard location) const
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

void PositionWithBitboards::MakeMove(Bitmove move)
{
    *extras_history_insertion_index_++ = boards_[BOARD_IDX_EXTRAS];
    const Bitboard& current_extras =
        *(extras_history_insertion_index_ - 1);  // Moving side, en-passant etc. will be changed in
                                                 // position. Hence getting unaltered state from history.

    const bool white_to_move = WhiteToMove();
    const std::size_t attacking_side = white_to_move ? BOARD_IDX_WHITE : BOARD_IDX_BLACK;
    const std::size_t attacking_piece = (move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE;
    const std::size_t attacking_piece_index = attacking_side + attacking_piece;
    const std::size_t captured_piece_kind = (move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE;

    const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
    const Bitboard target = BOARD_ONE << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
    const Bitboard source_and_target = source | target;

    boards_[BOARD_IDX_EXTRAS] ^= BOARD_MASK_WHITE_TURN;
    boards_[BOARD_IDX_EXTRAS] &= ~(BOARD_MASK_EN_PASSANT | BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE |
                                   BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE);

    boards_[attacking_side] ^= source_and_target;

    // revoke castling rights
    // TODO: This is ugly. Simplify.
    const bool someone_can_still_castle = current_extras & BOARD_MASK_CASTLING;
    if (someone_can_still_castle)
    {
        // revoke castling right due to moving own pieces
        const bool is_king_move = attacking_piece == KING;
        if (is_king_move)
        {
            if (white_to_move)
            {
                boards_[BOARD_IDX_EXTRAS] &=
                    ~(BOARD_VALUE_CASTLING_WHITE_KINGSIDE | BOARD_VALUE_CASTLING_WHITE_QUEENSIDE);
            }
            else
            {
                boards_[BOARD_IDX_EXTRAS] &=
                    ~(BOARD_VALUE_CASTLING_BLACK_KINGSIDE | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
            }
        }
        else if (attacking_piece == ROOK)
        {
            if (white_to_move)
            {
                if (source & H1)  // king rook moves
                {
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
                }
                else if (source & A1)  // queen rook moves
                {
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_QUEENSIDE;
                }
            }
            else
            {
                if (source & H8)  // king rook moves
                {
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
                }
                else if (source & A8)  // queen rook moves
                {
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
                }
            }
        }

        // take enemies castling rights due to capture of a rook
        const bool rook_is_captured = captured_piece_kind == ROOK;
        if (rook_is_captured)
        {
            switch (target)
            {
                case A1:
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_QUEENSIDE;
                    break;
                case H1:
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
                    break;
                case A8:
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
                    break;
                case H8:
                    boards_[BOARD_IDX_EXTRAS] &= ~BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
                    break;
            }
        }
    }

    const Bitmove move_type = move & MOVE_MASK_TYPE;
    switch (move_type)
    {
        case MOVE_VALUE_TYPE_QUIET_NON_PAWN: {
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[BOARD_IDX_EXTRAS]++;
            return;
        }
        case MOVE_VALUE_TYPE_CAPTURE: {
            const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
            const std::size_t captured_piece =
                defending_side + ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[defending_side] &= ~target;
            boards_[captured_piece] &= ~target;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            return;
        }
        case MOVE_VALUE_TYPE_PAWN_PUSH: {
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            return;
        }
        case MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH: {
            boards_[attacking_piece_index] ^= source_and_target;
            const Bitmove source_bit = move & MOVE_MASK_SOURCE;
            const Bitmove target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
            boards_[BOARD_IDX_EXTRAS] |= ((source_bit + target_bit) >> 1)
                                         << BOARD_SHIFT_EN_PASSANT;  // (source_bit+target_bit)/2
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            return;
        }
        case MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE: {
            const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
            const Bitboard en_passant_square = BOARD_ONE
                                               << ((current_extras & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT);
            const Bitboard en_passant_victim = white_to_move ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[defending_side] &= ~en_passant_victim;
            boards_[defending_side + PAWN] &= ~en_passant_victim;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            return;
        }
        case MOVE_VALUE_TYPE_KINGSIDE_CASTLING: {
            const Bitboard source_rook = target >> 1;
            const Bitboard target_rook = target << 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            const Bitboard board_mask_castling = attacking_side == BOARD_IDX_WHITE
                                                     ? BOARD_VALUE_CASTLING_WHITE_KINGSIDE
                                                     : BOARD_VALUE_CASTLING_BLACK_KINGSIDE;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[attacking_side] ^= source_and_target_rook_jump;
            boards_[attacking_side + ROOK] ^= source_and_target_rook_jump;
            boards_[BOARD_IDX_EXTRAS]++;
            boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
            boards_[BOARD_IDX_EXTRAS] |= BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE;
            return;
        }
        case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING: {
            const Bitboard source_rook = target << 2;
            const Bitboard target_rook = target >> 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            const Bitboard board_mask_castling = attacking_side == BOARD_IDX_WHITE
                                                     ? BOARD_VALUE_CASTLING_WHITE_QUEENSIDE
                                                     : BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[attacking_side] ^= source_and_target_rook_jump;
            boards_[attacking_side + ROOK] ^= source_and_target_rook_jump;
            boards_[BOARD_IDX_EXTRAS]++;
            boards_[BOARD_IDX_EXTRAS] &= ~board_mask_castling;
            boards_[BOARD_IDX_EXTRAS] |= BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE;
            return;
        }
        case MOVE_VALUE_TYPE_PROMOTION: {
            const std::size_t board_idx_added_piece_kind =
                attacking_side + ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
            boards_[attacking_piece_index] &= ~source;
            boards_[board_idx_added_piece_kind] |= target;
            boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_STATIC_PLIES;
            const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
            if (capture)
            {
                const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
                const std::size_t captured_piece = defending_side + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
                boards_[defending_side] &= ~target;
                boards_[captured_piece] &= ~target;
            }
            return;
        }
    }
}

void PositionWithBitboards::UnmakeMove(Bitmove move)
{
    extras_history_insertion_index_--;
    boards_[BOARD_IDX_EXTRAS] = *extras_history_insertion_index_;

    const bool white_to_move = WhiteToMove();
    const std::size_t attacking_side = white_to_move ? BOARD_IDX_WHITE : BOARD_IDX_BLACK;
    const std::size_t attacking_piece_index =
        attacking_side + ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE);

    const Bitboard source = BOARD_ONE << (move & MOVE_MASK_SOURCE);
    const Bitboard target = BOARD_ONE << ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET);
    const Bitboard source_and_target = source | target;

    boards_[attacking_side] ^= source_and_target;

    const Bitmove move_type = move & MOVE_MASK_TYPE;
    switch (move_type)
    {
        case MOVE_VALUE_TYPE_QUIET_NON_PAWN: {
            boards_[attacking_piece_index] ^= source_and_target;
            return;
        }
        case MOVE_VALUE_TYPE_CAPTURE: {
            const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
            const std::size_t captured_piece =
                defending_side + ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE);
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[defending_side] |= target;
            boards_[captured_piece] |= target;
            return;
        }
        case MOVE_VALUE_TYPE_PAWN_PUSH: {
            boards_[attacking_piece_index] ^= source_and_target;
            return;
        }
        case MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH: {
            boards_[attacking_piece_index] ^= source_and_target;
            return;
        }
        case MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE: {
            const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
            const Bitboard en_passant_square =
                BOARD_ONE << ((boards_[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT);
            const Bitboard en_passant_victim = white_to_move ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[defending_side] |= en_passant_victim;
            boards_[defending_side + PAWN] |= en_passant_victim;
            return;
        }
        case MOVE_VALUE_TYPE_KINGSIDE_CASTLING: {
            const Bitboard source_rook = target >> 1;
            const Bitboard target_rook = target << 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[attacking_side] ^= source_and_target_rook_jump;
            boards_[attacking_side + ROOK] ^= source_and_target_rook_jump;
            return;
        }
        case MOVE_VALUE_TYPE_QUEENSIDE_CASTLING: {
            const Bitboard source_rook = target << 2;
            const Bitboard target_rook = target >> 1;
            const Bitboard source_and_target_rook_jump = source_rook | target_rook;
            boards_[attacking_piece_index] ^= source_and_target;
            boards_[attacking_side] ^= source_and_target_rook_jump;
            boards_[attacking_side + ROOK] ^= source_and_target_rook_jump;
            return;
        }
        case MOVE_VALUE_TYPE_PROMOTION: {
            const std::size_t board_idx_added_piece_kind =
                attacking_side + ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION);
            boards_[attacking_piece_index] |= source;
            boards_[board_idx_added_piece_kind] &= ~target;
            const Bitmove capture = move & MOVE_MASK_CAPTURED_PIECE;
            if (capture)
            {
                const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;
                const std::size_t captured_piece = defending_side + (capture >> MOVE_SHIFT_CAPTURED_PIECE);
                boards_[defending_side] |= target;
                boards_[captured_piece] |= target;
            }
            return;
        }
    }
}

Bitboard& PositionWithBitboards::operator[](const std::size_t index)
{
    return boards_[index];
}

Bitboard PositionWithBitboards::operator[](const std::size_t index) const
{
    return boards_[index];
}

bool operator==(const PositionWithBitboards& a, const PositionWithBitboards& b)
{
    const bool boards_are_equal = a.boards_ == b.boards_;
    return boards_are_equal;
}

// Index corresponds to directions of "all_directions"
constexpr std::array<std::size_t, 8>
    dangerous_pieces_besides_queen_with_ray_style_attack{ROOK, BISHOP, ROOK, BISHOP, ROOK, BISHOP, ROOK, BISHOP};

bool PositionWithBitboards::DefendersKingIsInCheck() const
{
    const bool white_to_move = WhiteToMove();
    const std::size_t attacking_side = BOARD_IDX_BLACK + BOARD_IDX_BLACK_WHITE_DIFF * white_to_move;
    const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;

    const auto square_is_under_attack = [&](const Bitboard square) {
        // check for ray style attacks
        for (std::size_t idx = 0; idx < all_directions.size(); idx++)
        {
            const auto direction = all_directions[idx];
            const auto also_dangerous_piece = dangerous_pieces_besides_queen_with_ray_style_attack[idx];

            Bitboard attacker_location = SingleStep(square, direction);
            while (attacker_location)  // is on the board
            {
                const bool location_is_occupied_by_friendly_piece = attacker_location & boards_[defending_side];
                if (location_is_occupied_by_friendly_piece)
                {
                    break;  // this direction is safe
                }

                const bool location_is_occupied_by_enemy_piece = attacker_location & boards_[attacking_side];
                if (location_is_occupied_by_enemy_piece)
                {
                    const std::size_t piece_kind = GetPieceKind(attacking_side, attacker_location);
                    const bool piece_can_attack_from_this_angle =
                        (piece_kind == QUEEN) || (piece_kind == also_dangerous_piece);
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
        }

        const int square_bit = tzcnt(square);

        // check for knight attacks
        const bool knight_is_giving_check = knight_jumps[square_bit] & boards_[attacking_side + KNIGHT];
        if (knight_is_giving_check)
        {
            return true;
        }

        const bool pawn_is_giving_check =
            pawn_attacks[square_bit + (white_to_move * pawn_attacks_offset_for_white)] & boards_[attacking_side + PAWN];
        if (pawn_is_giving_check)
        {
            return true;
        }

        return false;
    };

    const Bitboard king_location = boards_[defending_side + KING];
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
