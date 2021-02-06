#ifndef SEACH_PRINCIPAL_VARIATION_H
#define SEACH_PRINCIPAL_VARIATION_H

#include "bitboard/basic_type_declarations.h"

#include <array>

namespace Chess
{

static constexpr std::size_t kMaximumLengthOfPrincipalVariation{8};

/// @brief The principal variation is stored as a triangular table.
///
/// It has space for the best line and all currently investigated sublines.
/// The table is concatenated to a single array. Its necessary length corresponds to the triangular number of the
/// maximum allowed search depth.
using PrincipalVariation =
    std::array<Bitmove, kMaximumLengthOfPrincipalVariation*(kMaximumLengthOfPrincipalVariation + 1) / 2>;

// TODO: Calculate at compile time
// Taken from https://www.chessprogramming.org/Triangular_PV-Table
/// @brief This helper function returns the index where the subline at given depth can be accessed in the
/// (logically triangular but in memory concatenated) principal variation table.
inline std::size_t GetSublineIndexAtDepth(const std::size_t depth)
{
    return depth * (2 * kMaximumLengthOfPrincipalVariation + 1 - depth) / 2;
}

}  // namespace Chess

#endif
