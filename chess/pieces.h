#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "alpha_beta/evaluation.h"
#include "chess/i_square.h"

#include <memory>

namespace Chess {

class Empty : public Square {
 public:
  Empty(const AlphaBeta::Player /*unused*/ = {}) {}
  ~Empty() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& /*unused*/) override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Pawn : public Square {
 public:
  Pawn(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Pawn() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
  bool en_passant_{false};
};

class Knight : public Square {
 public:
  Knight(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Knight() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
};

class Bishop : public Square {
 public:
  Bishop(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Bishop() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
};

class Rook : public Square {
 public:
  Rook(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Rook() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
  bool castling_{true};
};

class Queen : public Square {
 public:
  Queen(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Queen() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
};

class King : public Square {
 public:
  King(const AlphaBeta::Player side = {}) : side_(side) {}
  ~King() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player = {}) override final;
  std::ostream& print(std::ostream& stream) const override final;

 private:
  AlphaBeta::Player side_{};
  bool castling_{true};
};

using KingPtr = std::unique_ptr<King>;
using QueenPtr = std::unique_ptr<Queen>;
using RookPtr = std::unique_ptr<Rook>;
using BishopPtr = std::unique_ptr<Bishop>;
using KnightPtr = std::unique_ptr<Knight>;
using PawnPtr = std::unique_ptr<Pawn>;

}  // namespace Chess

#endif  // CHESS_PIECES_H
