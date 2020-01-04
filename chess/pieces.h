#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include "alpha_beta/evaluation.h"
#include "chess/i_square.h"
#include "chess/state.h"

#include <memory>
#include <vector>

namespace Chess {

class Empty : public ISquare {
 public:
  Empty(const AlphaBeta::Player /*unused*/ = {}) {}
  ~Empty() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& /*unused*/) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Pawn : public ISquare {
 public:
  Pawn(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Pawn() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

 private:
  AlphaBeta::Player side_{};
  bool en_passant_{false};
};

class Knight : public ISquare {
 public:
  Knight(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Knight() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

 private:
  AlphaBeta::Player side_{};
};

class Bishop : public ISquare {
 public:
  Bishop(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Bishop() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

 private:
  AlphaBeta::Player side_{};
};

class Rook : public ISquare {
 public:
  Rook(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Rook() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

 private:
  AlphaBeta::Player side_{};
  bool castling_{true};
};

class Queen : public ISquare {
 public:
  Queen(const AlphaBeta::Player side = {}) : side_(side) {}
  ~Queen() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

 private:
  AlphaBeta::Player side_{};
};

class King : public ISquare {
 public:
  King(const AlphaBeta::Player side = {}) : side_(side) {}
  ~King() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player = {}) override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;

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
