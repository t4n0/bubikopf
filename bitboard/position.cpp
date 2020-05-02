#include "bitboard/position.h"

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

}  // namespace Chess
