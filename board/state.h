#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "board/board.h"

#include <iostream>
#include <optional>

namespace Chess {

struct Castling {
  bool queenside{true};
  bool kingside{true};
};

class State {
 public:
  State();

  Board board_{};
  int static_plies_{0};
  int plies_{0};
  Player turn_{Player::max};
  std::optional<Coordinate> en_passant_{};

  std::vector<State> FindPlies() const;
  Castling GetCastling(const Player player) const;
  void SetCastling(const Player player, const Castling castling);

 private:
  Castling castling_black_{true, true};
  Castling castling_white_{true, true};
};

std::ostream& operator<<(std::ostream& stream, const State& state);

}  // namespace Chess

#endif
