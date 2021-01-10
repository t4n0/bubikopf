#ifndef BITBOARD_LOOKUP_TABLE_UTILITIES_H
#define BITBOARD_LOOKUP_TABLE_UTILITIES_H

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