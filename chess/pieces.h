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
  ~Empty() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& /*unused*/) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Piece : public ISquare {
 public:
  Piece(const AlphaBeta::Player side) : side_(side) {}
  ~Piece() override;

  AlphaBeta::Player side_{};
};

class Pawn : public Piece {
 public:
  Pawn(const AlphaBeta::Player side) : Piece(side) {}
  ~Pawn() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Knight : public Piece {
 public:
  Knight(const AlphaBeta::Player side) : Piece(side) {}
  ~Knight() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Bishop : public Piece {
 public:
  Bishop(const AlphaBeta::Player side) : Piece(side) {}
  ~Bishop() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Rook : public Piece {
 public:
  Rook(const AlphaBeta::Player side) : Piece(side) {}
  ~Rook() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class Queen : public Piece {
 public:
  Queen(const AlphaBeta::Player side) : Piece(side) {}
  ~Queen() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

class King : public Piece {
 public:
  King(const AlphaBeta::Player side) : Piece(side) {}
  ~King() override final {}

  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindMoves(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
  std::unique_ptr<ISquare> clone() const override final;
};

}  // namespace Chess

#endif  // CHESS_PIECES_H
