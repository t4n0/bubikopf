#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "alpha_beta/evaluation.h"
#include "alpha_beta/minimax.h"
#include "chess/pieces.h"

#include <array>
#include <iostream>
#include <vector>

namespace Chess {

using Column = int8_t;
using Row = int8_t;

struct Coordinate {
  Column col{};
  Row row{};

  Coordinate& operator+=(const Coordinate& rhs);
  Coordinate& operator-=(const Coordinate& rhs);
};
using Coordinates = std::vector<Coordinate>;

bool operator==(const Coordinate& a, const Coordinate& b);
bool operator!=(const Coordinate& a, const Coordinate& b);

Coordinate operator+(Coordinate a, const Coordinate& b);
Coordinate operator-(Coordinate a, const Coordinate& b);

std::size_t ToIdx(const Coordinate coor);
bool IsOnTheBoard(const Coordinate coordinate);

class Board {
 public:
  Board();

  const SquarePtr& Get(const Coordinate coor) const;
  void Set(const Coordinate coor, SquarePtr&& piece);

  std::array<std::unique_ptr<Square>, 64> squares_;
};

std::ostream& operator<<(std::ostream& stream, const Board& piece);

class State {
 public:
  Board board_{};
  int static_plies_{};
  int plies_{};
  GameTree::Player turn_{};
};

std::ostream& operator<<(std::ostream& stream, const State& state);

struct PlacedPiece {
  Coordinate coordinate{};
  SquarePtr piece{};
};
using PlacedPieces = std::vector<PlacedPiece>;

using Node = GameTree::Node<State>;
using NodePtr = std::unique_ptr<Node>;

}  // namespace Chess

#endif
