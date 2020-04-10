#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "board/i_square.h"

#include <array>
#include <iostream>
#include <optional>
#include <vector>

namespace Chess {

struct Coordinate {
  int8_t col{};
  int8_t row{};

  Coordinate& operator+=(const Coordinate& rhs);
  Coordinate& operator-=(const Coordinate& rhs);
};
using Coordinates = std::vector<Coordinate>;

bool operator==(const Coordinate& a, const Coordinate& b);
bool operator!=(const Coordinate& a, const Coordinate& b);

Coordinate operator+(Coordinate a, const Coordinate& b);
Coordinate operator-(Coordinate a, const Coordinate& b);

std::size_t ToIdx(const std::string& uci_square);
std::size_t ToIdx(const char file, const int8_t rank);
std::size_t ToIdx(const Coordinate coor);
Coordinate ToCoor(const std::size_t idx);
std::string ToUciSquare(const std::size_t idx);
bool IsOnTheBoard(const Coordinate coordinate);

class Board {
 public:
  ISquarePtr Get(const Coordinate coor) const;
  ISquarePtr Get(const std::size_t idx) const;
  void Set(const Coordinate coor, const ISquarePtr square);
  void Set(const std::size_t idx, const ISquarePtr square);
  void SwapSquares(const std::size_t a, const std::size_t b);

  std::array<ISquarePtr, 64> squares_{};
};

std::ostream& operator<<(std::ostream& stream, const Board& piece);

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
