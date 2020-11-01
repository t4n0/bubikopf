#ifndef BITBOARD_MOVE_H
#define BITBOARD_MOVE_H

#include "bitboard/basic_type_declarations.h"

#include <limits>
#include <array>

namespace Chess
{

constexpr Bitmove MOVE_ONES = std::numeric_limits<Bitmove>::max();

// clang-format off
constexpr Bitmove MOVE_MASK_SOURCE =                   0b00000000'00000000'00000000'00111111;
constexpr Bitmove MOVE_MASK_TARGET =                   0b00000000'00000000'00001111'11000000;
constexpr Bitmove MOVE_MASK_MOVED_PIECE =              0b00000000'00011100'00000000'00000000;
constexpr Bitmove MOVE_MASK_CAPTURED_PIECE =           0b00000000'11100000'00000000'00000000;
constexpr Bitmove MOVE_MASK_PROMOTION =                0b00000111'00000000'00000000'00000000;
constexpr Bitmove MOVE_MASK_TYPE =                     0b01111000'00000000'00000000'00000000;
constexpr Bitmove MOVE_MASK_UNUSED =                   0b10000000'00000011'11110000'00000000;

constexpr Bitmove MOVE_VALUE_TYPE_QUIET_NON_PAWN =     0b00000000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_CAPTURE =            0b00001000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PAWN_PUSH =          0b00010000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH =   0b00011000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE = 0b00100000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_KINGSIDE_CASTLING =  0b00101000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_QUEENSIDE_CASTLING = 0b00111000'00000000'00000000'00000000;
constexpr Bitmove MOVE_VALUE_TYPE_PROMOTION =          0b01010000'00000000'00000000'00000000;
// clang-format on

constexpr std::array<const char* const, 8> MOVE_VALUE_LABELS = {
    "QUIET_NON_PAWN",
    "CAPTURE",
    "PAWN_PUSH",
    "PAWN_DOUBLE_PUSH",
    "EN_PASSENT_CAPTURE",
    "KINGSIDE_CASTLING",
    "QUEENSIDE_CASTLING",
    "PROMOTION",
};

constexpr int MOVE_SHIFT_TARGET = 6;
constexpr int MOVE_SHIFT_STATIC = 12;
constexpr int MOVE_SHIFT_MOVED_PIECE = 18;
constexpr int MOVE_SHIFT_CAPTURED_PIECE = 21;
constexpr int MOVE_SHIFT_PROMOTION = 24;
constexpr int MOVE_SHIFT_TYPE = 27;

constexpr std::size_t NO_CAPTURE = 0;    // for clarity in tests
constexpr std::size_t NO_PROMOTION = 0;  // for clarity in tests

inline Bitmove ComposeMove(const Bitmove source,
                           const Bitmove target,
                           const Bitmove moved_piece,
                           const Bitmove captured_piece,
                           const Bitmove promotion,
                           const Bitmove move_type)
{
    return source |                                         //
           (target << MOVE_SHIFT_TARGET) |                  //
           (moved_piece << MOVE_SHIFT_MOVED_PIECE) |        //
           (captured_piece << MOVE_SHIFT_CAPTURED_PIECE) |  //
           (promotion << MOVE_SHIFT_PROMOTION) |            //
           move_type;
}

}  // namespace Chess

#endif
