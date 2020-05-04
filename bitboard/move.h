#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"

#include <cstdint>
#include <limits>

namespace Chess {

constexpr move_t MOVE_ONES = std::numeric_limits<move_t>::max();

// clang-format off
constexpr move_t MOVE_MASK_SOURCE =                   0b00000000'00000000'00000000'00111111;
constexpr move_t MOVE_MASK_TARGET =                   0b00000000'00000000'00001111'11000000;
constexpr move_t MOVE_MASK_STATIC =                   0b00000000'00000011'11110000'00000000;
constexpr move_t MOVE_MASK_MOVED_PIECE =              0b00000000'00011100'00000000'00000000;
constexpr move_t MOVE_MASK_CAPTURED_PIECE =           0b00000000'11100000'00000000'00000000;
constexpr move_t MOVE_MASK_PROMOTION =                0b00000111'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_TYPE =                     0b01111000'00000000'00000000'00000000;
constexpr move_t MOVE_MASK_UNUSED =                   0b10000000'00000000'00000000'00000000;

constexpr move_t MOVE_VALUE_TYPE_QUIET_NON_PAWN =     0b00000000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_CAPTURE =            0b00001000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_PAWN_PUSH =          0b00010000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH =   0b00011000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE = 0b00100000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_KINGSIDE =           0b00101000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_QUEENSIDE =          0b00111000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_PROMOTION =          0b01000000'00000000'00000000'00000000;
constexpr move_t MOVE_VALUE_TYPE_PROMOTION_CAPTURE =  0b01001000'00000000'00000000'00000000;
// clang-format on

constexpr int MOVE_SHIFT_TARGET = 6;
constexpr int MOVE_SHIFT_STATIC = 12;
constexpr int MOVE_SHIFT_MOVED_PIECE = 18;
constexpr int MOVE_SHIFT_CAPTURED_PIECE = 21;
constexpr int MOVE_SHIFT_PROMOTION = 24;

}  // namespace Chess

#endif
