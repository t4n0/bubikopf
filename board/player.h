#ifndef BOARD_PLAYER_H
#define BOARD_PLAYER_H

namespace Chess {
enum class Player {
  min,  // flag for minimizing player (e.g turn, victory, etc.)
  max,  // flag for maximizing player (e.g turn, victory, etc.)
};
}

#endif
