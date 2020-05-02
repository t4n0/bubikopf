#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include "bitboard/move.h"

#include <array>

namespace Chess {
using move_list_t = std::array<move_t, 1000>;
}

#endif  // MOVE_LIST_H
