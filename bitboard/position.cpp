#include "bitboard/position.h"

#include "bitboard/lookup_table/knight.h"
#include "bitboard/lookup_table/pawn.h"
#include "bitboard/lookup_table/piece.h"
#include "bitboard/pieces.h"
#include "bitboard/shift.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <stdexcept>

namespace Chess
{

std::size_t Position::GetTotalPlies() const
{
    return (boards_[kExtrasBoard] & kBoardMaskTotalPlies) >> kBoardShiftTotalPlies;
}

Bitmove Position::GetPieceKind(const std::size_t side, const Bitboard location) const
{
    if (boards_[side + kPawn] & location)
    {
        return kPawn;
    }
    if (boards_[side + kKnight] & location)
    {
        return kKnight;
    }
    if (boards_[side + kBishop] & location)
    {
        return kBishop;
    }
    if (boards_[side + kRook] & location)
    {
        return kRook;
    }
    if (boards_[side + kQueen] & location)
    {
        return kQueen;
    }
    if (boards_[side + kKing] & location)
    {
        return kKing;
    }
    return kNoPiece;
}

Bitboard Position::MakeMove(Bitmove move)
{
    const Bitboard current_extras = boards_[kExtrasBoard];  // These extras correspond to move from function
                                                            // parameter including unaltered en passant information etc.

    const std::size_t attacking_piece = ExtractMovedPiece(move);
    const std::size_t attacking_piece_index = attacking_side_ + attacking_piece;

    const Bitboard source = Bitboard{1} << ExtractSource(move);
    const Bitboard target = Bitboard{1} << ExtractTarget(move);
    const Bitboard source_and_target = source | target;

    constexpr Bitboard obsolete_extras_from_last_move{kBoardMaskEnPassant | kKingsideCastlingOnLastMove |
                                                      kQueensideCastlingOnLastMove};
    boards_[kExtrasBoard] &= ~obsolete_extras_from_last_move;
    boards_[kExtrasBoard] += kIncrementTotalPlies;

    boards_[attacking_side_] ^= source_and_target;
    boards_[attacking_piece_index] ^= source_and_target;

    const Bitmove move_type = move & kMoveMaskType;
    switch (move_type)
    {
        case kMoveTypeQuietNonPawn: {
            boards_[kExtrasBoard]++;
            break;
        }
        case kMoveTypeCapture: {
            const std::size_t captured_piece = defending_side_ + ExtractCapturedPiece(move);
            boards_[defending_side_] &= ~target;
            boards_[captured_piece] &= ~target;
            boards_[kExtrasBoard] &= ~kBoardMaskStaticPlies;
            break;
        }
        case kMoveTypePawnSinglePush: {
            boards_[kExtrasBoard] &= ~kBoardMaskStaticPlies;
            break;
        }
        case kMoveTypePawnDoublePush: {
            const Bitmove source_bit = ExtractSource(move);
            const Bitmove target_bit = ExtractTarget(move);
            boards_[kExtrasBoard] |= ((source_bit + target_bit) >> 1)
                                     << kBoardShiftEnPassant;  // (source_bit+target_bit)/2
            boards_[kExtrasBoard] &= ~kBoardMaskStaticPlies;
            break;
        }
        case kMoveTypeEnPassantCapture: {
            const Bitboard en_passant_square = Bitboard{1}
                                               << ((current_extras & kBoardMaskEnPassant) >> kBoardShiftEnPassant);
            const Bitboard en_passant_victim = white_to_move_ ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[defending_side_] &= ~en_passant_victim;
            boards_[defending_side_ + kPawn] &= ~en_passant_victim;
            boards_[kExtrasBoard] &= ~kBoardMaskStaticPlies;
            break;
        }
        case kMoveTypeKingsideCastling: {
            constexpr Bitboard white_rook_jump = F1 | H1;
            constexpr Bitboard black_rook_jump = F8 | H8;
            const Bitboard rook_jump_source_and_target = white_to_move_ ? white_rook_jump : black_rook_jump;
            boards_[attacking_side_] ^= rook_jump_source_and_target;
            boards_[attacking_side_ + kRook] ^= rook_jump_source_and_target;
            boards_[kExtrasBoard]++;
            boards_[kExtrasBoard] |= kKingsideCastlingOnLastMove;
            break;
        }
        case kMoveTypeQueensideCastling: {
            constexpr Bitboard white_rook_jump = A1 | D1;
            constexpr Bitboard black_rook_jump = A8 | D8;
            const Bitboard rook_jump_source_and_target = white_to_move_ ? white_rook_jump : black_rook_jump;
            boards_[attacking_side_] ^= rook_jump_source_and_target;
            boards_[attacking_side_ + kRook] ^= rook_jump_source_and_target;
            boards_[kExtrasBoard]++;
            boards_[kExtrasBoard] |= kQueensideCastlingOnLastMove;
            break;
        }
        case kMoveTypePromotion: {
            const std::size_t board_idx_added_piece_kind = attacking_side_ + ExtractPromotion(move);
            boards_[attacking_piece_index] &=
                ~source_and_target;  // pawn was moved to target as side effect of default operation earlier
            boards_[board_idx_added_piece_kind] |= target;
            boards_[kExtrasBoard] &= ~kBoardMaskStaticPlies;
            const Bitmove capture = move & kMoveMaskCapturedPiece;
            if (capture)
            {
                const std::size_t captured_piece = defending_side_ + (capture >> kMoveShiftCapturedPiece);
                boards_[defending_side_] &= ~target;
                boards_[captured_piece] &= ~target;
            }
            break;
        }
    }

    // revoke castling rights
    const bool someone_can_still_castle = current_extras & kBoardMaskCastling;
    if (someone_can_still_castle)
    {
        constexpr Bitboard white_queenside_castling_squares = A1 | E1;
        if (source_and_target & white_queenside_castling_squares)
        {
            boards_[kExtrasBoard] &= ~kCastlingWhiteQueenside;
        }
        constexpr Bitboard white_kingside_castling_squares = E1 | H1;
        if (source_and_target & white_kingside_castling_squares)
        {
            boards_[kExtrasBoard] &= ~kCastlingWhiteKingside;
        }
        constexpr Bitboard black_queenside_castling_squares = A8 | E8;
        if (source_and_target & black_queenside_castling_squares)
        {
            boards_[kExtrasBoard] &= ~kCastlingBlackQueenside;
        }
        constexpr Bitboard black_kingside_castling_squares = E8 | H8;
        if (source_and_target & black_kingside_castling_squares)
        {
            boards_[kExtrasBoard] &= ~kCastlingBlackKingside;
        }
    }

    white_to_move_ = !white_to_move_;
    attacking_side_ ^= kToggleSide;
    defending_side_ ^= kToggleSide;

    return current_extras;
}

void Position::UnmakeMove(Bitmove move, Bitboard saved_extras)
{
    boards_[kExtrasBoard] = saved_extras;
    white_to_move_ = !white_to_move_;
    attacking_side_ ^= kToggleSide;
    defending_side_ ^= kToggleSide;

    const std::size_t attacking_piece_index = attacking_side_ + ExtractMovedPiece(move);

    const Bitboard source = Bitboard{1} << ExtractSource(move);
    const Bitboard target = Bitboard{1} << ExtractTarget(move);
    const Bitboard source_and_target = source | target;

    boards_[attacking_side_] ^= source_and_target;
    boards_[attacking_piece_index] ^= source_and_target;

    const Bitmove move_type = move & kMoveMaskType;
    switch (move_type)
    {
        case kMoveTypeCapture: {
            const std::size_t captured_piece = defending_side_ + ExtractCapturedPiece(move);
            boards_[defending_side_] |= target;
            boards_[captured_piece] |= target;
            return;
        }

        case kMoveTypeEnPassantCapture: {
            const Bitboard en_passant_square =
                Bitboard{1} << ((boards_[kExtrasBoard] & kBoardMaskEnPassant) >> kBoardShiftEnPassant);
            const Bitboard en_passant_victim = white_to_move_ ? en_passant_square >> 8 : en_passant_square << 8;
            boards_[defending_side_] |= en_passant_victim;
            boards_[defending_side_ + kPawn] |= en_passant_victim;
            return;
        }
        case kMoveTypeKingsideCastling: {
            constexpr Bitboard white_rook_jump = F1 | H1;
            constexpr Bitboard black_rook_jump = F8 | H8;
            const Bitboard rook_jump_source_and_target = white_to_move_ ? white_rook_jump : black_rook_jump;
            boards_[attacking_side_] ^= rook_jump_source_and_target;
            boards_[attacking_side_ + kRook] ^= rook_jump_source_and_target;
            return;
        }
        case kMoveTypeQueensideCastling: {
            constexpr Bitboard white_rook_jump = A1 | D1;
            constexpr Bitboard black_rook_jump = A8 | D8;
            const Bitboard rook_jump_source_and_target = white_to_move_ ? white_rook_jump : black_rook_jump;
            boards_[attacking_side_] ^= rook_jump_source_and_target;
            boards_[attacking_side_ + kRook] ^= rook_jump_source_and_target;
            return;
        }
        case kMoveTypePromotion: {
            boards_[attacking_piece_index] &=
                ~target;  // pawns were set on target and source as side effect of default operation
            const std::size_t board_idx_added_piece_kind = attacking_side_ + ExtractPromotion(move);
            boards_[board_idx_added_piece_kind] &= ~target;
            const Bitmove capture = move & kMoveMaskCapturedPiece;
            if (capture)
            {
                const std::size_t captured_piece = defending_side_ + (capture >> kMoveShiftCapturedPiece);
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

bool Position::IsKingInCheck(const std::size_t defending_side) const
{
    const std::size_t attacking_side = defending_side ^ kToggleSide;

    const auto ray_check_given =
        [&](const Bitboard square, const std::size_t direction, const std::size_t dangerous_piece_besides_queen) {
            Bitboard attacker_location = SingleStep(square, direction);
            while (attacker_location)  // is on the board
            {
                const bool attacker_location_is_occupied =
                    attacker_location & (boards_[kBlackBoard] | boards_[kWhiteBoard]);
                if (attacker_location_is_occupied)
                {
                    const bool dangerous_piece_on_attacker_location =
                        attacker_location &
                        (boards_[attacking_side + kQueen] | boards_[attacking_side + dangerous_piece_besides_queen]);
                    if (dangerous_piece_on_attacker_location)
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
        const bool rook_or_queen_aligned_on_rank_or_file =
            kRookAttacks[square_bit] & (boards_[attacking_side + kRook] | boards_[attacking_side + kQueen]);
        if (rook_or_queen_aligned_on_rank_or_file)
        {
            for (const auto direction : {kWest, kSouth, kEast, kNorth})
            {
                if (ray_check_given(square, direction, kRook))
                {
                    return true;
                }
            }
        }

        // ray style checks diagonally
        const bool bishop_or_queen_aligned_diagonally =
            kBishopAttacks[square_bit] & (boards_[attacking_side + kBishop] | boards_[attacking_side + kQueen]);
        if (bishop_or_queen_aligned_diagonally)
        {
            for (const auto direction : {kNorthEast, kNorthWest, kSouthEast, kSouthWest})
            {
                if (ray_check_given(square, direction, kBishop))
                {
                    return true;
                }
            }
        }

        // knight checks
        const bool knight_is_giving_check = kKnightJumps[square_bit] & boards_[attacking_side + kKnight];
        if (knight_is_giving_check)
        {
            return true;
        }

        // pawn checks
        const bool pawn_is_giving_check =
            kPawnAttacks[square_bit + (white_to_move_ * kPawnAttacksLookupTableOffsetForWhite)] &
            boards_[attacking_side + kPawn];
        if (pawn_is_giving_check)
        {
            return true;
        }

        // king checks
        const bool king_is_giving_check = kKingAttacks[square_bit] & boards_[attacking_side + kKing];
        if (king_is_giving_check)
        {
            return true;
        }

        return false;
    };

    const Bitboard king_location = boards_[defending_side + kKing];
    if (square_is_under_attack(king_location))
    {
        return true;
    }

    const bool defending_side_just_castled =
        boards_[kExtrasBoard] & (kKingsideCastlingOnLastMove | kQueensideCastlingOnLastMove);
    if (defending_side_just_castled)
    {
        // pass through square
        const bool defending_side_just_castled_kingside = boards_[kExtrasBoard] & kKingsideCastlingOnLastMove;
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
