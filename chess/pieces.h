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

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& /*unused*/) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Piece : public ISquare {
 public:
  Piece(const AlphaBeta::Player player) : owner_(player) {}
  ~Piece() override;

  AlphaBeta::Player owner_{};
};

class Pawn : public Piece {
 public:
  Pawn(const AlphaBeta::Player player) : Piece(player) {}
  ~Pawn() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Knight : public Piece {
 public:
  Knight(const AlphaBeta::Player player) : Piece(player) {}
  ~Knight() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Bishop : public Piece {
 public:
  Bishop(const AlphaBeta::Player player) : Piece(player) {}
  ~Bishop() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Rook : public Piece {
 public:
  Rook(const AlphaBeta::Player player) : Piece(player) {}
  ~Rook() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Queen : public Piece {
 public:
  Queen(const AlphaBeta::Player player) : Piece(player) {}
  ~Queen() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class King : public Piece {
 public:
  King(const AlphaBeta::Player player) : Piece(player) {}
  ~King() override final {}

  Square GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

}  // namespace Chess

#endif  // CHESS_PIECES_H
