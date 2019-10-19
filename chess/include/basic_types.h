#ifndef CHESS_BASIC_TYPES_H
#define CHESS_BASIC_TYPES_H

#include <alpha_beta/evaluation.h>

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Chess {

enum class Piece {
  e,  // empty square = default
  p,  // black pawn
  r,  // black rook
  n,  // black knight
  b,  // black bishop
  q,  // black queen
  k,  // black king
  P,  // white pawn
  R,  // white rook
  N,  // white knight
  B,  // white bishop
  Q,  // white queen
  K,  // white king
};

using Column = int8_t;
using Row = int8_t;

struct Coordinate {
  Column col{};
  Row row{};
};
using Coordinates = std::vector<Coordinate>;

class Board {
 public:
  Piece Get(const Coordinate coor) const;
  void Set(const Coordinate coor, const Piece piece);

  std::array<std::array<Piece, 8>, 8> data_{};
};

struct PlacedPiece {
  Coordinate coordinate{};
  Piece piece{};
};
using PlacedPieces = std::vector<PlacedPiece>;

bool IsAPieceOfSide(const Piece piece, const GameTree::Player side);

bool operator==(const Coordinate& a, const Coordinate& b);
bool operator!=(const Coordinate& a, const Coordinate& b);

std::ostream& operator<<(std::ostream& stream, const Piece piece);
std::ostream& operator<<(std::ostream& stream, const Board& piece);

}  // namespace Chess

#endif
