#include "chess/state.h"
#include "chess/pieces.h"

namespace Chess {

bool operator==(const Coordinate& a, const Coordinate& b) {
  return (a.col == b.col) && (a.row == b.row);
}

bool operator!=(const Coordinate& a, const Coordinate& b) { return !(a == b); }

Coordinate& Coordinate::operator+=(const Coordinate& rhs) {
  col += rhs.col;
  row += rhs.row;
  return *this;
}

Coordinate& Coordinate::operator-=(const Coordinate& rhs) {
  col -= rhs.col;
  row -= rhs.row;
  return *this;
}

Coordinate operator+(Coordinate lhs, const Coordinate& rhs) {
  return lhs += rhs;
}

Coordinate operator-(Coordinate lhs, const Coordinate& rhs) {
  return lhs -= rhs;
}

std::size_t ToIdx(const Coordinate coor) {
  return static_cast<std::size_t>(coor.col + 8 * coor.row);
}

Coordinate ToCoor(const std::size_t idx) {
  const std::div_t division = std::div(static_cast<int>(idx), 8);
  return {static_cast<int8_t>(division.rem),
          static_cast<int8_t>(division.quot)};
}

bool IsOnTheBoard(const Coordinate coor) {
  return (coor.row >= 0 && coor.row < 8 && coor.col >= 0 && coor.col < 8);
}

Board::Board() {
  for (auto& square : squares_) {
    square = std::make_unique<Empty>();
  }
}

const Square& Board::Get(const Coordinate coor) const {
  if (IsOnTheBoard(coor)) {
    return *squares_[ToIdx(coor)];
  } else {
    throw std::out_of_range{"Board::Get: Out of range."};
  }
}

void Board::Set(const Coordinate coor, SquarePtr&& piece) {
  if (IsOnTheBoard(coor)) {
    squares_[ToIdx(coor)].swap(piece);
  } else {
    throw std::out_of_range{"Board::Set: Out of range."};
  }
}

std::ostream& operator<<(std::ostream& stream, const Board& board) {
  for (std::size_t idx{0}; idx < board.squares_.size(); idx++) {
    stream << *board.squares_[idx] << " ";
    if ((idx + 1) % 8 == 0) {
      stream << '\n';
    }
  }
  stream << '\n';

  return stream;
}

std::ostream& operator<<(std::ostream& stream, const State& state) {
  stream << state.board_;
  return stream;
}

}  // namespace Chess
