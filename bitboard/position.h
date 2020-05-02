#ifndef BITBOARD_POSITION_H
#define BITBOARD_POSITION_H

#include "bitboard/move.h"
#include "bitboard/pieces.h"

#include <array>
#include <cstdint>
#include <limits>

namespace Chess {

using bitboard_t = uint64_t;

constexpr bitboard_t BOARD_ONES = std::numeric_limits<bitboard_t>::max();
constexpr bitboard_t BOARD_ZEROS = 0;

constexpr std::size_t BOARD_IDX_EXTRAS = 0;
constexpr std::size_t BOARD_IDX_BLACK =
    1;  // Add to piece constant (e.g. PAWN) to get actual bitboard index
constexpr std::size_t BOARD_IDX_WHITE = 9;
constexpr std::size_t BOARD_IDX_ATTACK = 7;

constexpr bitboard_t BOARD_MASK_STATIC_PLIES =
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00111111;
constexpr bitboard_t BOARD_MASK_EN_PASSENT =
    0b00000000'00000000'00000000'00000000'00000000'00000000'00111111'00000000;
constexpr bitboard_t BOARD_MASK_CASTLING =
    0b00000000'00000000'00000000'00000000'00000000'00001111'00000000'00000000;
constexpr bitboard_t BOARD_MASK_WHITE_TURN =
    0b00000000'00000000'00000000'00000000'00001000'00000000'00000000'00000000;
constexpr bitboard_t BOARD_MASK_UNUSED =
    0b11111111'11111111'11111111'11111111'11110111'11110000'11000000'11000000;

constexpr int BOARD_SHIFT_EN_PASSENT = 8;
constexpr int BOARD_SHIFT_CASTLING = 16;
constexpr int BOARD_SHIFT_WHITE_TURN = 24;

class PositionWithBitboards {
 public:
  void MakeMove(move_t move);
  void UnmakeMove(move_t move);
  bool WhiteToMove();

  bitboard_t& operator[](const std::size_t index);
  bitboard_t operator[](const std::size_t index) const;

  std::array<bitboard_t, 16> boards;
};

}  // namespace Chess

#endif
