#include "bitboard/position.h"

#include "bitboard/pieces.h"
#include "bitboard/squares.h"
#include "bitboard/shift.h"

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
    extras_history_[extras_history_insertion_index_] = boards_[BOARD_IDX_EXTRAS];
    extras_history_insertion_index_++;
    const Bitboard& current_extras =
        extras_history_[extras_history_insertion_index_ - 1];  // Moving side, en-passant etc. will be changed in
                                                               // position. Hence getting unaltered state from history.

    const bool white_to_move = WhiteToMove();
    const std::size_t attacking_side = white_to_move ? BOARD_IDX_WHITE : BOARD_IDX_BLACK;
    const std::size_t attacking_piece = (move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE;
    const std::size_t attacking_piece_index = attacking_side + attacking_piece;

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
    boards_[BOARD_IDX_EXTRAS] = extras_history_[extras_history_insertion_index_];

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
    const bool iterators_are_equal = a.extras_history_insertion_index_ == b.extras_history_insertion_index_;
    const bool histories_are_equal = std::equal(begin(a.extras_history_),
                                                begin(a.extras_history_) + a.extras_history_insertion_index_,
                                                begin(b.extras_history_));  // Don't consider obsolete data at
                                                                            // insertion index and beyond

    return boards_are_equal && iterators_are_equal && histories_are_equal;
}

PositionWithBitboards SetUpStandardStartPosition()
{
    PositionWithBitboards position{};
    position[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING | BOARD_MASK_WHITE_TURN;

    // white pieces
    position[BOARD_IDX_WHITE] = RANK_2 | RANK_1;
    position[BOARD_IDX_WHITE + PAWN] = RANK_2;
    position[BOARD_IDX_WHITE + ROOK] = A1 | H1;
    position[BOARD_IDX_WHITE + KNIGHT] = B1 | G1;
    position[BOARD_IDX_WHITE + BISHOP] = C1 | F1;
    position[BOARD_IDX_WHITE + QUEEN] = D1;
    position[BOARD_IDX_WHITE + KING] = E1;

    // black pieces
    position[BOARD_IDX_BLACK] = RANK_7 | RANK_8;
    position[BOARD_IDX_BLACK + PAWN] = RANK_7;
    position[BOARD_IDX_BLACK + ROOK] = A8 | H8;
    position[BOARD_IDX_BLACK + KNIGHT] = B8 | G8;
    position[BOARD_IDX_BLACK + BISHOP] = C8 | F8;
    position[BOARD_IDX_BLACK + QUEEN] = D8;
    position[BOARD_IDX_BLACK + KING] = E8;

    return position;
}

// Index corresponds to directions of "all_directions"
constexpr std::array<std::size_t, 8>
    dangerous_pieces_besides_queen_with_ray_style_attack{ROOK, BISHOP, ROOK, BISHOP, ROOK, BISHOP, ROOK, BISHOP};

bool PositionWithBitboards::DefendersKingIsInCheck() const
{
    const std::size_t attacking_side = BOARD_IDX_BLACK + BOARD_IDX_BLACK_WHITE_DIFF * WhiteToMove();
    const std::size_t defending_side = BOARD_IDX_BLACK_WHITE_SUM - attacking_side;

    // check for ray style attacks
    for (std::size_t idx = 0; idx < all_directions.size(); idx++)
    {
        const auto direction = all_directions[idx];
        const auto also_dangerous_piece = dangerous_pieces_besides_queen_with_ray_style_attack[idx];

        Bitboard attacker_location = SingleStep(boards_[defending_side + KING], direction);
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
            }

            attacker_location = SingleStep(attacker_location, direction);  // next iteration
        }
    }

    // check for knight attacks
    for (const auto knight_jump : knight_jumps)
    {
        const Bitboard attacker_location = KnightJump(boards_[defending_side + KING], knight_jump);
        if (attacker_location)  // is on the board
        {
            const bool location_is_occupied_by_enemy_knight = attacker_location & boards_[attacking_side + KNIGHT];
            if (location_is_occupied_by_enemy_knight)
            {
                return true;
            }
        }
    }

    // TODO: Merge with ray style attacks above
    // check for pawn attacks
    constexpr std::array<std::size_t, 2> pawn_attack_angles_for_black_king{south_east, south_west};
    constexpr std::array<std::size_t, 2> pawn_attack_angles_for_white_king{north_east, north_west};
    const auto& attack_directions =
        WhiteToMove() ? pawn_attack_angles_for_black_king : pawn_attack_angles_for_white_king;
    for (const auto pawn_attack_direction : attack_directions)
    {
        const Bitboard attacker_location = SingleStep(boards_[defending_side + KING], pawn_attack_direction);
        const bool attacker_location_is_occupied_by_enemy_pawn = attacker_location & boards_[attacking_side + PAWN];
        if (attacker_location_is_occupied_by_enemy_pawn)
        {
            return true;
        }
    }

    return false;
}

}  // namespace Chess
