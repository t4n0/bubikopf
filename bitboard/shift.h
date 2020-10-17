#ifndef BITBOARD_SHIFT_H
#define BITBOARD_SHIFT_H

#include "bitboard/basic_type_declarations.h"

namespace Chess
{

const std::size_t west = 0;
const std::size_t north_west = 1;
const std::size_t north = 2;
const std::size_t north_east = 3;
const std::size_t east = 4;
const std::size_t south_east = 5;
const std::size_t south = 6;
const std::size_t south_west = 7;

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
Bitboard Shift(const Bitboard value, const std::size_t direction);

}  // namespace Chess

#endif