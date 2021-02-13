#ifndef SEACH_PRINCIPAL_VARIATION_H
#define SEACH_PRINCIPAL_VARIATION_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/move.h"
#include "bitboard/uci_conversion.h"

#include <array>
#include <sstream>

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

inline void ClearSublines(PrincipalVariation& principal_variation)
{
    for (std::size_t index{GetSublineIndexAtDepth(1)}; index < principal_variation.size(); index++)
    {
        principal_variation[index] = kBitNullMove;
    }
}

inline std::string ToString(const PrincipalVariation& principal_variation)
{
    constexpr const char* const separator = " ";                                 // one space
    constexpr const char* const empty_promotion = " ";                           // one space
    constexpr const char* const uci_move_placeholder_with_max_length = "     ";  // five spaces

    std::stringstream stream{};

    int line{0};  // 0 is the main line, 1 the subline after first move of main line, etc.
    for (std::size_t index{0}; index < principal_variation.size(); index++)
    {
        const bool end_of_line_reached = (index == GetSublineIndexAtDepth(line + 1));
        if (end_of_line_reached)
        {
            const bool is_main_line{line == 0};
            if (is_main_line)
            {
                stream << "(main line)";
            }
            line++;
            stream << '\n';
            for (int count{0}; count < line; count++)
            {
                stream << uci_move_placeholder_with_max_length << separator;
            }
        }
        const std::string uci_move{ToUciString(principal_variation[index])};
        stream << uci_move << separator;
        const bool is_move_without_promotion{uci_move.size() == 4};
        if (is_move_without_promotion)
        {
            stream << empty_promotion;  // Make sure printout is aligned for promotions, too.
        }
    }
    stream << '\n';

    return stream.str();
}

}  // namespace Chess

#endif
