#ifndef CHESS_I_SQUARE_H
#define CHESS_I_SQUARE_H

#include "alpha_beta/evaluation.h"

#include <memory>
#include <vector>

namespace Chess {

class State;

enum class Square {
  WhitePawn,
  WhiteKnight,
  WhiteBishop,
  WhiteRook,
  WhiteQueen,
  WhiteKing,
  BlackPawn,
  BlackKnight,
  BlackBishop,
  BlackRook,
  BlackQueen,
  BlackKing,
  Empty,
};

class ISquare {
 public:
  virtual ~ISquare() = default;

  virtual Square GetId() const = 0;
  virtual float GetValue() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const AlphaBeta::Player& player) const = 0;
  virtual std::vector<State> FindMoves(const std::size_t idx,
                                       const State& state) const = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const ISquare& square) {
  return square.print(stream);
}

using ISquarePtr = std::shared_ptr<const ISquare>;

}  // namespace Chess

#endif  // CHESS_I_SQUARE_H
