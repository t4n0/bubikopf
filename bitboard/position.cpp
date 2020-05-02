#include "bitboard/position.h"

namespace Chess {

bool PositionWithBitboards::WhiteToMove() {
  return boards[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN;
}

}  // namespace Chess
