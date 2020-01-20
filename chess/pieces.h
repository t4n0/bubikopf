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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
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

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const AlphaBeta::Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class SquareBehaviourPool {
 public:
  ISquarePtr GetEmpty() const;
  ISquarePtr GetPawn(const AlphaBeta::Player player) const;
  ISquarePtr GetKnight(const AlphaBeta::Player player) const;
  ISquarePtr GetBishop(const AlphaBeta::Player player) const;
  ISquarePtr GetRook(const AlphaBeta::Player player) const;
  ISquarePtr GetQueen(const AlphaBeta::Player player) const;
  ISquarePtr GetKing(const AlphaBeta::Player player) const;

 private:
  Empty empty_square_{};
  Pawn black_pawn_{AlphaBeta::Player::min};
  Knight black_knight_{AlphaBeta::Player::min};
  Bishop black_bishop_{AlphaBeta::Player::min};
  Rook black_rook_{AlphaBeta::Player::min};
  Queen black_queen_{AlphaBeta::Player::min};
  King black_king_{AlphaBeta::Player::min};
  Pawn white_pawn_{AlphaBeta::Player::max};
  Knight white_knight_{AlphaBeta::Player::max};
  Bishop white_bishop_{AlphaBeta::Player::max};
  Rook white_rook_{AlphaBeta::Player::max};
  Queen white_queen_{AlphaBeta::Player::max};
  King white_king_{AlphaBeta::Player::max};
};

}  // namespace Chess

#endif  // CHESS_PIECES_H
