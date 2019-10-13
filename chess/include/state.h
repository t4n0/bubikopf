#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include <chess/basic_types.h>
#include <game_tree/minimax.h>

#include <algorithm>
#include <iostream>

namespace Chess {

class State {
 public:
  Piece GetPieceFrom(const File file, const Rank rank) const;
  void SetSquareTo(const File file, const Rank rank, const Piece piece);

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
