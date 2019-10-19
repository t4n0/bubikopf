#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include <alpha_beta/evaluation.h>
#include <alpha_beta/minimax.h>

#include <array>
#include <iostream>
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

std::ostream& operator<<(std::ostream& stream, const Piece piece);

using Column = int8_t;
using Row = int8_t;

struct Coordinate {
  Column col{};
  Row row{};
};
using Coordinates = std::vector<Coordinate>;

bool operator==(const Coordinate& a, const Coordinate& b);
bool operator!=(const Coordinate& a, const Coordinate& b);

class Board {
 public:
  Piece Get(const Coordinate coor) const;
  void Set(const Coordinate coor, const Piece piece);

  std::array<std::array<Piece, 8>, 8> data_{};
};

std::ostream& operator<<(std::ostream& stream, const Board& piece);

class State {
 public:
  // FEN example of starting position:
  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  Board board_{};
  GameTree::Player turn_{};
  std::array<bool, 4> castling_{};
  Coordinate en_passant_{};
  int static_plies_{};
  int plies_{};
};

std::ostream& operator<<(std::ostream& stream, const State& state);

struct PlacedPiece {
  Coordinate coordinate{};
  Piece piece{};
};
using PlacedPieces = std::vector<PlacedPiece>;

using Node = GameTree::Node<State>;
using NodePtr = std::unique_ptr<Node>;

bool IsAPieceOfSide(const Piece piece, const GameTree::Player side);
bool IsOnTheBoard(const Coordinate coordinate);

}  // namespace Chess

#endif
