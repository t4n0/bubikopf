#ifndef BITBOARD_SHIFT_H
#define BITBOARD_SHIFT_H

#include "bitboard/basic_type_declarations.h"

namespace Chess
{

/// @brief Indizes to acces different aspects of a single piece step (except knights).
const std::size_t west = 0;
const std::size_t north_west = 1;
const std::size_t north = 2;
const std::size_t north_east = 3;
const std::size_t east = 4;
const std::size_t south_east = 5;
const std::size_t south = 6;
const std::size_t south_west = 7;

/// @brief Indizes to acces aspects of a knight jump.
///
/// north_west means two seps north, one step west. In contrast west_north means two steps west and one step north.
const std::size_t knight_north_west = 0;
const std::size_t knight_north_east = 1;
const std::size_t knight_east_north = 2;
const std::size_t knight_east_south = 3;
const std::size_t knight_south_east = 4;
const std::size_t knight_south_west = 5;
const std::size_t knight_west_south = 6;
const std::size_t knight_west_north = 7;

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
Bitboard SingleStep(const Bitboard value, const std::size_t direction);

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
Bitboard KnightJump(const Bitboard value, const std::size_t direction);

}  // namespace Chess

#endif