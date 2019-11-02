#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "alpha_beta/evaluation.h"

#include <memory>

namespace Chess {

class Square {
 public:
  virtual ~Square();

  virtual float GetValue() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const GameTree::Player& player) = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

std::ostream& operator<<(std::ostream& stream, const Square& square);

class Empty : public Square {
 public:
  Empty(const GameTree::Player /*unused*/ = {}) {}
  ~Empty() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const GameTree::Player& /*unused*/) override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Pawn : public Square {
 public:
  Pawn(const GameTree::Player side = {}) : side_(side) {}
  ~Pawn() override final {}

  float GetValue() const override final;
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

  float GetValue() const override final;
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

  float GetValue() const override final;
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

  float GetValue() const override final;
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

  float GetValue() const override final;
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

  float GetValue() const override final;
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

}  // namespace Chess

#endif  // CHESS_PIECES_H
