#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"

#include <cstdint>
#include <limits>

namespace Chess {

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

// clang-format off
constexpr move_t MOVE_MASK_EXTRAS_IS_INVERTIBLE =           0b10000000'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_CAPTURED_PIECE =          0b01110000'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_MOVED_PIECE =             0b00001110'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_PROMOTION =               0b00000001'11000000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_IS_EN_PASSENT =           0b00000000'00100000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_IS_KINGSIDE_CASLTLING =   0b00000000'00010000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_IS_QUEENSIDE_CASLTLING =  0b00000000'00001000'00000000'00000000;
constexpr move_t MOVE_MASK_EXTRAS_UNUSED =                  0b00000000'00000100'00000000'00000000;
// clang-format on

constexpr move_t MOVE_MASK_EXTRAS_IS_CASLTLING =
    MOVE_MASK_EXTRAS_IS_KINGSIDE_CASLTLING |
    MOVE_MASK_EXTRAS_IS_QUEENSIDE_CASLTLING;

constexpr int MOVE_SHIFT_TARGET = 6;
constexpr int MOVE_SHIFT_STATIC = 12;
constexpr int MOVE_SHIFT_EXTRAS_CAPTURED_PIECE = 28;
constexpr int MOVE_SHIFT_EXTRAS_MOVED_PIECE = 25;
constexpr int MOVE_SHIFT_EXTRAS_PROMOTION = 22;

}  // namespace Chess

#endif
