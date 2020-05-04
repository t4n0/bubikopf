#ifndef BITBOARD_POSITION_H
#define BITBOARD_POSITION_H

#include "bitboard/basic_type_declarations.h"
#include "bitboard/move.h"
#include "bitboard/pieces.h"

#include <array>
#include <cstdint>
#include <limits>

namespace Chess {

constexpr bitboard_t BOARD_ZEROS = 0;
constexpr bitboard_t BOARD_ONE = 1;
constexpr bitboard_t BOARD_ONES = std::numeric_limits<bitboard_t>::max();

constexpr std::size_t BOARD_IDX_EXTRAS = 0;
constexpr std::size_t BOARD_IDX_BLACK = 1;
constexpr std::size_t BOARD_IDX_WHITE = 9;
constexpr std::size_t BOARD_IDX_ATTACK = 7;
constexpr std::size_t BOARD_IDX_BLACK_AND_WHITE_ADDED =
    BOARD_IDX_BLACK + BOARD_IDX_WHITE;

constexpr bitboard_t BOARD_MASK_STATIC_PLIES =
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00111111;
constexpr bitboard_t BOARD_MASK_EN_PASSENT =
    0b00000000'00000000'00000000'00000000'00000000'00000000'00111111'00000000;
constexpr bitboard_t BOARD_MASK_CASTLING =
    0b00000000'00000000'00000000'00000000'00000000'00001111'00000000'00000000;
constexpr bitboard_t BOARD_MASK_WHITE_TURN =
    0b00000000'00000000'00000000'00000000'00000001'00000000'00000000'00000000;
constexpr bitboard_t BOARD_MASK_UNUSED =
    0b11111111'11111111'11111111'11111111'11111110'11110000'11000000'11000000;

constexpr bitboard_t BOARD_VALUE_CASTLING_WHITE_KINGSIDE =
    0b00000000'00000000'00000000'00000000'00000000'00001000'00000000'00000000;
constexpr bitboard_t BOARD_VALUE_CASTLING_WHITE_QUEENSIDE =
    0b00000000'00000000'00000000'00000000'00000000'00000100'00000000'00000000;
constexpr bitboard_t BOARD_VALUE_CASTLING_BLACK_KINGSIDE =
    0b00000000'00000000'00000000'00000000'00000000'00000010'00000000'00000000;
constexpr bitboard_t BOARD_VALUE_CASTLING_BLACK_QUEENSIDE =
    0b00000000'00000000'00000000'00000000'00000000'00000001'00000000'00000000;

constexpr int BOARD_SHIFT_EN_PASSENT = 8;
constexpr int BOARD_SHIFT_CASTLING = 16;

class PositionWithBitboards {
 public:
  PositionWithBitboards() {}
  PositionWithBitboards(const std::array<bitboard_t, 17>& boards)
      : boards_(boards) {}
  void MakeMove(move_t move);
  void UnmakeMove(move_t move);
  bool WhiteToMove();

  bitboard_t& operator[](const std::size_t index);
  bitboard_t operator[](const std::size_t index) const;

  std::array<bitboard_t, 17> boards_{};
  std::array<bitboard_t, 40> extras_history_{};
  std::array<bitboard_t, 40>::iterator extras_history_insertion_iterator_{
      extras_history_.begin()};
};

}  // namespace Chess

#endif
