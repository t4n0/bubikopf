#ifndef BOARD_PIECE_H
#define BOARD_PIECE_H

#include "board/i_square.h"
#include "board/state.h"

#include <memory>
#include <vector>

namespace Chess {

class Empty : public ISquare {
 public:
  ~Empty() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& /*unused*/) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Piece : public ISquare {
 public:
  Piece(const Player player) : owner_(player) {}
  ~Piece() override;

  Player owner_{};
};

class Pawn : public Piece {
 public:
  Pawn(const Player player) : Piece(player) {}
  ~Pawn() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Knight : public Piece {
 public:
  Knight(const Player player) : Piece(player) {}
  ~Knight() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Bishop : public Piece {
 public:
  Bishop(const Player player) : Piece(player) {}
  ~Bishop() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Rook : public Piece {
 public:
  Rook(const Player player) : Piece(player) {}
  ~Rook() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class Queen : public Piece {
 public:
  Queen(const Player player) : Piece(player) {}
  ~Queen() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class King : public Piece {
 public:
  King(const Player player) : Piece(player) {}
  ~King() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

class SquareBehaviourPool {
 public:
  ISquarePtr GetEmpty() const;
  ISquarePtr GetPawn(const Player player) const;
  ISquarePtr GetKnight(const Player player) const;
  ISquarePtr GetBishop(const Player player) const;
  ISquarePtr GetRook(const Player player) const;
  ISquarePtr GetQueen(const Player player) const;
  ISquarePtr GetKing(const Player player) const;

 private:
  Empty empty_square_{};
  Pawn black_pawn_{Player::min};
  Knight black_knight_{Player::min};
  Bishop black_bishop_{Player::min};
  Rook black_rook_{Player::min};
  Queen black_queen_{Player::min};
  King black_king_{Player::min};
  Pawn white_pawn_{Player::max};
  Knight white_knight_{Player::max};
  Bishop white_bishop_{Player::max};
  Rook white_rook_{Player::max};
  Queen white_queen_{Player::max};
  King white_king_{Player::max};
};

}  // namespace Chess

#endif
