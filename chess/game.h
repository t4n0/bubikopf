#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess/pieces.h"
#include "chess/state.h"

namespace Chess {

inline const SquareBehaviourPool POOL{};

inline State SetUpEmptyBoard() {
  State state{POOL};
  for (std::size_t idx{0}; idx < state.board_.squares_.size(); idx++) {
    state.board_.Set(idx, POOL.GetEmpty());
  }

  return state;
}

}  // namespace Chess

#endif  // CHESS_GAME_H
