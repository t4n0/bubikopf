#ifndef BITBOARD_SHIFT_H
#define BITBOARD_SHIFT_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/lookup_table/knight.h"
#include "bitboard/lookup_table/ray.h"

#include <array>
#include <cmath>

namespace Chess
{

constexpr std::array<std::size_t, 8>
    all_directions{kWest, kNorthWest, kNorth, kNorthEast, kEast, kSouthEast, kSouth, kSouthWest};
constexpr std::array<std::size_t, 4> bishop_directions{kNorthWest, kNorthEast, kSouthWest, kSouthEast};
constexpr std::array<std::size_t, 4> rook_directions{kWest, kNorth, kEast, kSouth};

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
inline Bitboard SingleStep(const Bitboard value, const std::size_t direction)
{
    const int shift = kStepBits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & kLegalAreasWithoutWrapping[direction];
}

/// @brief Shifts the given bitboard in the given direction and NULLs the board if "wrap around" occurs.
/// @pre Assumes that only a single bit is set
inline Bitboard RuntimeKnightJump(const Bitboard value, const std::size_t direction)
{
    const int shift = kJumpBits[direction];
    const Bitboard shifted_with_wrap = shift > 0 ? value << shift : value >> std::abs(shift);
    return shifted_with_wrap & kLegalLandingAreasWithoutWrapping[direction];
}

}  // namespace Chess

#endif