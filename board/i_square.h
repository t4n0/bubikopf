#ifndef BOARD_I_SQUARE_H
#define BOARD_I_SQUARE_H

#include "board/player.h"

#include <memory>
#include <vector>

namespace Chess {

class Position;

enum class SquareId {
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

  virtual SquareId GetId() const = 0;
  virtual float GetValue() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual bool IsOfSide(const Player& player) const = 0;
  virtual std::vector<Position> FindPlies(const std::size_t idx,
                                          const Position& position) const = 0;
  virtual std::ostream& print(std::ostream& stream) const = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const ISquare& square) {
  return square.print(stream);
}

using ISquarePtr = const ISquare*;

}  // namespace Chess

#endif
