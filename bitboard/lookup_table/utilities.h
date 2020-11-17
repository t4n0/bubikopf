#ifndef BITBOARD_LOOKUP_TABLE_UTILITIES_H
#define BITBOARD_LOOKUP_TABLE_UTILITIES_H

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

}  // namespace Chess

#endif