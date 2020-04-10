#ifndef BOARD_PIECE_H
#define BOARD_PIECE_H

#include "board/i_square.h"
#include "board/state.h"

#include <memory>
#include <vector>

namespace Chess {

class Empty : public ISquare {
 private:
  Empty() {}

 public:
  static ISquarePtr Make();
  Empty(const Empty&) = delete;
  Empty operator=(const Empty&) = delete;
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
 private:
  Pawn(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  Pawn(const Pawn&) = delete;
  Pawn operator=(const Pawn&) = delete;
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
 private:
  Knight(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  Knight(const Knight&) = delete;
  Knight operator=(const Knight&) = delete;
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
 private:
  Bishop(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  Bishop(const Bishop&) = delete;
  Bishop operator=(const Bishop&) = delete;
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
 private:
  Rook(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  Rook(const Rook&) = delete;
  Rook operator=(const Rook&) = delete;
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
 private:
  Queen(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  Queen(const Queen&) = delete;
  Queen operator=(const Queen&) = delete;
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
 private:
  King(const Player player) : Piece(player) {}

 public:
  static ISquarePtr OfSide(const Player player);
  King(const King&) = delete;
  King operator=(const King&) = delete;
  ~King() override final {}

  SquareId GetId() const override final;
  float GetValue() const override final;
  bool IsEmpty() const override final;
  bool IsOfSide(const Player& player) const override final;
  std::vector<State> FindPlies(const std::size_t idx,
                               const State& state) const override final;
  std::ostream& print(std::ostream& stream) const override final;
};

}  // namespace Chess

#endif
