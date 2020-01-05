#include "chess/state.h"
#include "chess/pieces.h"

#include <algorithm>
#include <memory>

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

Board::Board(const Board& board) {
  std::transform(board.squares_.begin(), board.squares_.end(), squares_.begin(),
                 [](const ISquarePtr& square) { return square->clone(); });
}

Board& Board::operator=(Board other) {
  this->swap(other);
  return *this;
}

void Board::swap(Board& other) { squares_.swap(other.squares_); }

const ISquare& Board::Get(const Coordinate coor) const {
  if (IsOnTheBoard(coor)) {
    return *squares_[ToIdx(coor)];
  } else {
    throw std::out_of_range{"Board::Get: Out of range."};
  }
}

void Board::Set(const Coordinate coor, ISquarePtr&& piece) {
  if (IsOnTheBoard(coor)) {
    squares_[ToIdx(coor)].swap(piece);
  } else {
    throw std::out_of_range{"Board::Set: Out of range."};
  }
}

Castling State::GetCastling(const AlphaBeta::Player player) const {
  return player == AlphaBeta::Player::max ? castling_white_ : castling_black_;
}

void State::SetCastling(const AlphaBeta::Player player,
                        const Castling castling) {
  if (player == AlphaBeta::Player::max) {
    castling_white_ = castling;
  } else {
    castling_black_ = castling;
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
