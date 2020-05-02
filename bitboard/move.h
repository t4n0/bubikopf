#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include <cstdint>
#include <limits>

namespace Chess {

using move_t = uint32_t;

/// All bits set to one for move_t
constexpr move_t MOVE_ONES = std::numeric_limits<move_t>::max();
/// All bits set to zero for move_t
constexpr move_t MOVE_ZERO = std::numeric_limits<move_t>::max();

/// @brief Location of bits for index of source square
constexpr move_t MOVE_MASK_SOURCE = 0b00000000'00000000'00000000'00111111;
/// @brief Location of bits for index of target square
constexpr move_t MOVE_MASK_TARGET = 0b00000000'00000000'00001111'11000000;
/// @brief Location of bits for static plies with respect to 50 move rule
constexpr move_t MOVE_MASK_STATIC = 0b00000000'00000011'11110000'00000000;
/// @brief Location of bits for flags for promotion, en passent, etc.
constexpr move_t MOVE_MASK_EXTRAS = 0b11111111'11111100'00000000'00000000;

constexpr int MOVE_SHIFT_TARGET = 6;
constexpr int MOVE_SHIFT_STATIC = 12;
constexpr int MOVE_SHIFT_EXTRAS = 18;

// clang-format off
constexpr move_t MOVE_MASK_EXTRAS_IS_CAPTURE =     0b10000000'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_CAPTURED_PIECE = 0b01110000'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_PROMOTION =      0b00001110'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_CASTLING =       0b00000001'10000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_IS_EN_PASSENT =  0b00000000'01000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_UNUSED =         0b00000000'00111100'00000000'00000000;
// clang-format on

}  // namespace Chess

#endif
