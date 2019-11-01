#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "alpha_beta/evaluation.h"
#include "alpha_beta/minimax.h"

#include <array>
#include <iostream>
#include <vector>

namespace Chess {

class Square {
 public:
  virtual ~Square();

  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const GameTree::Player& player) = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& stream, const Square& square);

class Empty : public Square {
 public:
  Empty(const GameTree::Player /*unused*/ = {}) {}
  ~Empty() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& /*unused*/) override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Pawn : public Square {
 public:
  Pawn(const GameTree::Player side = {}) : side_(side) {}
  ~Pawn() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
  bool en_passant_{false};
};

class Knight : public Square {
 public:
  Knight(const GameTree::Player side = {}) : side_(side) {}
  ~Knight() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
};

class Bishop : public Square {
 public:
  Bishop(const GameTree::Player side = {}) : side_(side) {}
  ~Bishop() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
};

class Rook : public Square {
 public:
  Rook(const GameTree::Player side = {}) : side_(side) {}
  ~Rook() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
  bool castling_{true};
};

class Queen : public Square {
 public:
  Queen(const GameTree::Player side = {}) : side_(side) {}
  ~Queen() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
};

class King : public Square {
 public:
  King(const GameTree::Player side = {}) : side_(side) {}
  ~King() override final {}

  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& player = {}) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  GameTree::Player side_{};
  bool castling_{true};
};

using SquarePtr = std::unique_ptr<Square>;
using KingPtr = std::unique_ptr<King>;
using QueenPtr = std::unique_ptr<Queen>;
using RookPtr = std::unique_ptr<Rook>;
using BishopPtr = std::unique_ptr<Bishop>;
using KnightPtr = std::unique_ptr<Knight>;
using PawnPtr = std::unique_ptr<Pawn>;

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

  std::array<std::unique_ptr<Square>, 64> squares_;

  const SquarePtr& Get(const Coordinate coor) const;
  void Set(const Coordinate coor, SquarePtr&& piece);
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

template <typename T>
struct PlacedPiece {
  Coordinate coordinate{};
  T piece{};
};

using Node = GameTree::Node<State>;
using NodePtr = std::unique_ptr<Node>;

}  // namespace Chess

#endif
