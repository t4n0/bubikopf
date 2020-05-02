#include "bitboard/position.h"

#include "bitboard/pieces.h"

#include <iostream>

namespace Chess {

bool PositionWithBitboards::WhiteToMove() {
  return boards[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN;
}

void PositionWithBitboards::MakeMove(move_t /*unused*/) {
  // TODO: implement
}

void PositionWithBitboards::UnmakeMove(move_t /*unused*/) {
  // TODO: implement
}

bitboard_t& PositionWithBitboards::operator[](const std::size_t index) {
  return boards[index];
}

bitboard_t PositionWithBitboards::operator[](const std::size_t index) const {
  return boards[index];
}

}  // namespace Chess
