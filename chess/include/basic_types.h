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

enum class Piece : int {
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
  e,  // empty square
};

const std::map<Piece, std::string> PieceAbbreviationMap{
    {Piece::p, "p"}, {Piece::r, "r"}, {Piece::n, "n"}, {Piece::b, "b"},
    {Piece::q, "q"}, {Piece::k, "k"}, {Piece::P, "P"}, {Piece::R, "R"},
    {Piece::N, "N"}, {Piece::B, "B"}, {Piece::Q, "Q"}, {Piece::K, "K"},
    {Piece::e, "_"},
};

using Code = std::variant<Piece, int>;
using CodedRank = std::vector<Code>;
using BoardContainer = std::array<CodedRank, 8>;
using File = char;
using Rank = int;

struct Coordinate {
  File file_{};
  Rank rank_{};
};
using Coordinates = std::vector<Coordinate>;

struct PlacedPiece {
  Coordinate coordinate_{};
  Piece piece_{};
};
using PlacedPieces = std::vector<PlacedPiece>;

const std::map<File, int> FileMap{
    {'a', 1}, {'b', 2}, {'c', 3}, {'d', 4},
    {'e', 5}, {'f', 6}, {'g', 7}, {'h', 8},
};

const std::map<Rank, std::size_t> RankMap{
    {1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {7, 1}, {8, 0},
};

const std::vector<File> FILES{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const std::vector<Rank> RANKS{8, 7, 6, 5, 4, 3, 2, 1};

inline bool IsAPieceOfSide(const Piece piece, const GameTree::Player side) {
  if (piece == Piece::e) {
    return false;
  }

  const bool piece_is_black{piece >= Piece::p && piece <= Piece::k};
  const bool piece_is_white{piece >= Piece::P && piece <= Piece::K};

  return (piece_is_black && side == GameTree::Player::min) ||
         (piece_is_white && side == GameTree::Player::max);
};

struct GetCoveredColumnsVisitor {
  int operator()(const Piece /*unused*/) { return 1; }
  int operator()(const int value) { return value; }
};
inline int GetCoveredColumns(const Code& code) {
  return std::visit(GetCoveredColumnsVisitor{}, code);
}

struct GetPieceVisitor {
  Piece operator()(const Piece piece) { return piece; }
  Piece operator()(const int /*unused*/) { return Piece::e; }
};
inline Piece GetPiece(const Code& code) {
  return std::visit(GetPieceVisitor{}, code);
}

struct IsAPieceVisitor {
  bool operator()(const Piece piece) { return piece != Piece::e; }
  bool operator()(const int /*unused*/) { return false; }
};
inline bool IsAPiece(const Code& code) {
  return std::visit(IsAPieceVisitor{}, code);
}

struct GetEmptySquaresVisitor {
  int operator()(const Piece piece) { return int{piece == Piece::e}; }
  int operator()(const int value) { return value; }
};
inline int GetEmptySquares(const Code& code) {
  return std::visit(GetEmptySquaresVisitor{}, code);
}

struct SprintSquareCodeVisitor {
  std::string operator()(const Piece piece) {
    return PieceAbbreviationMap.at(piece);
  }
  std::string operator()(const int value) { return std::to_string(value); }
};
inline std::string SprintSquareCode(const Code& code) {
  return std::visit(SprintSquareCodeVisitor{}, code);
}

inline bool operator==(const Coordinate& a, const Coordinate& b) {
  return ((a.file_ == b.file_) && (a.rank_ == b.rank_));
}
inline bool operator!=(const Coordinate& a, const Coordinate& b) {
  return !(a == b);
}

inline std::ostream& operator<<(std::ostream& stream, const Piece piece) {
  stream << PieceAbbreviationMap.at(piece);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Code code) {
  stream << SprintSquareCode(code);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const CodedRank& codes) {
  stream << "size " << codes.size() << '\n';
  for (auto code : codes) {
    stream << code << ' ';
  }
  return stream;
}

}  // namespace Chess

#endif
