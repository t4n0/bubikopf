#ifndef CHESS_I_SQUARE_H
#define CHESS_I_SQUARE_H

#include "alpha_beta/evaluation.h"

#include <memory>

namespace Chess {

class Square {
 public:
  virtual ~Square() = default;

  virtual float GetValue() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const GameTree::Player& player) = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const Square& square) {
  return square.print(stream);
}

using SquarePtr = std::unique_ptr<Square>;

}  // namespace Chess

#endif  // CHESS_I_SQUARE_H
