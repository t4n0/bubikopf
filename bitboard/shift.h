#ifndef BITBOARD_SHIFT_H
#define BITBOARD_SHIFT_H

#include "bitboard/basic_type_declarations.h"

#include <array>

namespace Chess
{

/// @brief Indizes to acces different aspects of a single piece step (except knights).
constexpr std::size_t west = 0;
constexpr std::size_t north_west = 1;
constexpr std::size_t north = 2;
constexpr std::size_t north_east = 3;
constexpr std::size_t east = 4;
constexpr std::size_t south_east = 5;
constexpr std::size_t south = 6;
constexpr std::size_t south_west = 7;

/// @brief Indizes to acces aspects of a knight jump.
///
/// north_west means two seps north, one step west. In contrast west_north means two steps west and one step north.
constexpr std::size_t knight_north_west = 0;
constexpr std::size_t knight_north_east = 1;
constexpr std::size_t knight_east_north = 2;
constexpr std::size_t knight_east_south = 3;
constexpr std::size_t knight_south_east = 4;
constexpr std::size_t knight_south_west = 5;
constexpr std::size_t knight_west_south = 6;
constexpr std::size_t knight_west_north = 7;

constexpr std::array<std::size_t, 8>
    all_directions{west, north_west, north, north_east, east, south_east, south, south_west};
constexpr std::array<std::size_t, 4> bishop_directions{north_west, north_east, south_west, south_east};
constexpr std::array<std::size_t, 4> rook_directions{west, north, east, south};
constexpr std::array<std::size_t, 8> knight_jumps{knight_north_west,
                                                  knight_north_east,
                                                  knight_east_north,
                                                  knight_east_south,
                                                  knight_south_east,
                                                  knight_south_west,
                                                  knight_west_south,
                                                  knight_west_north};

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
Bitboard SingleStep(const Bitboard value, const std::size_t direction);

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
Bitboard KnightJump(const Bitboard value, const std::size_t direction);

}  // namespace Chess

#endif