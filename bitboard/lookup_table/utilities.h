#ifndef BITBOARD_LOOKUP_TABLE_UTILITIES_H
#define BITBOARD_LOOKUP_TABLE_UTILITIES_H

#include "bitboard/squares.h"

#include <array>

namespace Chess
{

/// @brief Calculates absolute value of input
template <int positive_or_negative>
struct Absolute
{
    constexpr static int value = positive_or_negative < 0 ? -positive_or_negative : positive_or_negative;
};

/// @brief Converts input source_bit to a bitboard
template <int source_bit>
struct ToBitboard
{
    constexpr static Bitboard value = Bitboard{1} << source_bit;
};

/// @brief A function mimicking a for loop over all squares at compile time.
///
/// It calls itself recursively. The final call fills value at index 63. The second to last index 62, etc.
template <template <Bitboard> typename SquareAssignment, std::size_t index = 0>
constexpr std::array<Bitboard, 64> LoopOverAllSquares()
{
    std::array<Bitboard, 64> array{};
    if constexpr (index < 63)  // terminate recursion at index 63
    {
        array = LoopOverAllSquares<SquareAssignment, index + 1>();
    }
    std::get<index>(array) = SquareAssignment<std::get<index>(kAllSquares)>::value;
    return array;
}

template <typename value_type, std::size_t size_a, std::size_t size_b>
constexpr std::array<value_type, size_a + size_b> ConcatenateArrays(std::array<value_type, size_a> a,
                                                                    std::array<value_type, size_b> b)
{
    std::array<value_type, size_a + size_b> concatenated_array{};
    for (std::size_t index{0}; index < size_a; index++)
    {
        concatenated_array[index] = a[index];
    }
    for (std::size_t index{0}; index < size_b; index++)
    {
        concatenated_array[index + size_a] = b[index];
    }
    return concatenated_array;
}

}  // namespace Chess

#endif