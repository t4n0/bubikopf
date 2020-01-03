#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "alpha_beta/evaluation.h"
#include "alpha_beta/minimax.h"
#include "chess/i_square.h"

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
Coordinate ToCoor(const std::size_t idx);
bool IsOnTheBoard(const Coordinate coordinate);

class Board {
 public:
  Board();

  const ISquare& Get(const Coordinate coor) const;
  void Set(const Coordinate coor, ISquarePtr&& piece);

  std::array<ISquarePtr, 64> squares_;
};

std::ostream& operator<<(std::ostream& stream, const Board& piece);

class State {
 public:
  Board board_{};
  int static_plies_{};
  int plies_{};
  AlphaBeta::Player turn_{};
};

std::ostream& operator<<(std::ostream& stream, const State& state);

using Node = AlphaBeta::Node<State>;
using NodePtr = std::unique_ptr<Node>;

}  // namespace Chess

#endif
