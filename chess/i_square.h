#ifndef CHESS_I_SQUARE_H
#define CHESS_I_SQUARE_H

#include "alpha_beta/evaluation.h"

#include <memory>
#include <vector>

namespace Chess {

class State;

class ISquare {
 public:
  virtual ~ISquare() = default;

  virtual float GetValue() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const AlphaBeta::Player& player) = 0;
  virtual std::vector<State> FindMoves(const std::size_t idx,
                                       const State& state) const = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const ISquare& square) {
  return square.print(stream);
}

using ISquarePtr = std::unique_ptr<ISquare>;

}  // namespace Chess

#endif  // CHESS_I_SQUARE_H
