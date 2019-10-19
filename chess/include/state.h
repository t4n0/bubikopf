#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include <alpha_beta/minimax.h>
#include <chess/basic_types.h>

#include <algorithm>
#include <iostream>

namespace Chess {

class State {
 public:
  Piece GetPieceFrom(const Column file, const Row rank) const;
  void SetSquareTo(const Column file, const Row rank, const Piece piece);

  // FEN example of starting position:
  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  BoardContainer board_{};
  GameTree::Player turn_{};
  std::array<bool, 4> castling_{};
  Coordinate en_passant_{};
  int static_plies_{};
  int plies_{};
};

using Node = GameTree::Node<State>;
using NodePtr = std::unique_ptr<Node>;

std::ostream& operator<<(std::ostream& stream, const State& state);

}  // namespace Chess

#endif
