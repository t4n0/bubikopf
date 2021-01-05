#ifndef SEACH_MATERIAL_DIFFERENCE_COMPARISON_H
#define SEACH_MATERIAL_DIFFERENCE_COMPARISON_H

#include "bitboard/move.h"
#include "evaluate/evaluate.h"

namespace Chess
{

constexpr std::array<Evaluation, 7> kPieceValues{
    kNullValue,
    kPawnValue,
    kKnightValue,
    kBishopValue,
    kRookValue,
    kQueenValue,
    kKingValue,
};

constexpr std::array<Evaluation, 7> kPromotionValues{
    kNullValue,
    kPawnValue,
    kKnightValue - kPawnValue,
    kBishopValue - kPawnValue,
    kRookValue - kPawnValue,
    kQueenValue - kPawnValue,
    kKingValue - kPawnValue,
};

/// @brief Helper function for sorting promising moves first during search.
///
/// Returns true when comparing e.g a capture with a quiet move as the former is more promising.
inline bool IsMaterialDifferenceGreater(const Bitmove a, const Bitmove b)
{
    return (kPieceValues[ExtractCapturedPiece(a)] + kPromotionValues[ExtractPromotion(a)]) >
           (kPieceValues[ExtractCapturedPiece(b)] + kPromotionValues[ExtractPromotion(b)]);
}

}  // namespace Chess

#endif
