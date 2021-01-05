#include "bitboard/move.h"

#include "bitboard/pieces.h"
#include "bitboard/squares.h"

#include <array>
#include <sstream>

namespace Chess
{

Bitmove ComposeMove(const Bitmove source,
                    const Bitmove target,
                    const Bitmove moved_piece,
                    const Bitmove captured_piece,
                    const Bitmove promotion,
                    const Bitmove move_type)
{
    return source |                                       //
           (target << kMoveShiftTarget) |                 //
           (moved_piece << kMoveShiftMovedPiece) |        //
           (captured_piece << kMoveShiftCapturedPiece) |  //
           (promotion << kMoveShiftPromotion) |           //
           move_type;
}

std::string ToString(const Bitmove move)
{
    constexpr std::array<const char* const, 8> move_type_labels{
        "QUIET_NON_PAWN",
        "CAPTURE",
        "PAWN_PUSH",
        "PAWN_DOUBLE_PUSH",
        "EN_PASSANT_CAPTURE",
        "KINGSIDE_CASTLING",
        "QUEENSIDE_CASTLING",
        "PROMOTION",
    };

    if (move == kBitNullMove)
    {
        return {};
    }

    const std::size_t source_bit = move & kMoveMaskSource;
    const std::string source = kSquareLabels.at(source_bit);

    const std::size_t target_bit = (move & kMoveMaskTarget) >> kMoveShiftTarget;
    const std::string target = kSquareLabels.at(target_bit);

    const std::size_t moved_piece_kind = (move & kMoveMaskMovedPiece) >> kMoveShiftMovedPiece;
    const char moved_piece = kPieceLabels.at(moved_piece_kind);

    const std::size_t captured_piece_kind = (move & kMoveMaskCapturedPiece) >> kMoveShiftCapturedPiece;
    const char captured_piece = kPieceLabels.at(captured_piece_kind);

    const std::size_t promotion_kind = (move & kMoveMaskPromotion) >> kMoveShiftPromotion;
    const char promotion = kPieceLabels.at(promotion_kind);

    const std::size_t move_value_kind = (move & kMoveMaskType) >> kMoveShiftType;
    const std::string move_value = move_type_labels.at(move_value_kind);

    std::stringstream move_print_out{};
    move_print_out << moved_piece << " on " << source << " to " << target << " promoting to " << promotion
                   << " capturing " << captured_piece << " with move value " << move_value << "\n";

    return move_print_out.str();
}

Bitmove ExtractSource(const Bitmove move)
{
    return move & kMoveMaskSource;
}

Bitmove ExtractTarget(const Bitmove move)
{
    return (move & kMoveMaskTarget) >> kMoveShiftTarget;
}

Bitmove ExtractMovedPiece(const Bitmove move)
{
    return (move & kMoveMaskMovedPiece) >> kMoveShiftMovedPiece;
}

Bitmove ExtractCapturedPiece(const Bitmove move)
{
    return (move & kMoveMaskCapturedPiece) >> kMoveShiftCapturedPiece;
}

Bitmove ExtractPromotion(const Bitmove move)
{
    return (move & kMoveMaskPromotion) >> kMoveShiftPromotion;
}

Bitmove ExtractType(const Bitmove move)
{
    return (move & kMoveMaskType) >> kMoveShiftType;
}

}  // namespace Chess
