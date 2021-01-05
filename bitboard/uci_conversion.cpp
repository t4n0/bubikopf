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
    uci_move << kSquareLabels.at(ExtractSource(move)) << kSquareLabels.at(ExtractTarget(move));

    const std::size_t promotion_kind = ExtractPromotion(move);
    if (promotion_kind)
    {
        uci_move << kPieceLabels.at(promotion_kind);
    }

    return uci_move.str();
}

}  // namespace Chess