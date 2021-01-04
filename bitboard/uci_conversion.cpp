#include "bitboard/uci_conversion.h"

#include "bitboard/move.h"
#include "bitboard/pieces.h"
#include "bitboard/squares.h"

#include <sstream>

namespace Chess
{

std::string ToUciString(const Bitmove move)
{
    if (move == kBitNullMove)
    {
        return kUciNullMove;
    }

    std::stringstream uci_move{};
    const std::size_t source_bit = move & kMoveMaskSource;
    const std::size_t target_bit = (move & kMoveMaskTarget) >> kMoveShiftTarget;
    uci_move << kSquareLabels.at(source_bit) << kSquareLabels.at(target_bit);
    const std::size_t promotion_kind = (move & kMoveMaskPromotion) >> kMoveShiftPromotion;
    if (promotion_kind)
    {
        uci_move << kPieceLabels.at(promotion_kind);
    }

    return uci_move.str();
}

}  // namespace Chess