#include "board/state.h"

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

std::size_t ToIdx(const std::string& uci_square) {
  const int8_t col = uci_square.at(0) - 'a';
  const int8_t row = 8 - (uci_square.at(1) - '0');
  return ToIdx(Coordinate{col, row});
}

std::size_t ToIdx(const char file, const int8_t rank) {
  const int8_t col = 8 - ('i' - file);
  const int8_t row = 8 - rank;
  return ToIdx(Coordinate{col, row});
}

std::size_t ToIdx(const Coordinate coor) {
  return static_cast<std::size_t>(coor.col + 8 * coor.row);
}

Coordinate ToCoor(const std::size_t idx) {
  const std::div_t division = std::div(static_cast<int>(idx), 8);
  return {static_cast<int8_t>(division.rem),
          static_cast<int8_t>(division.quot)};
}

std::string ToUciSquare(const std::size_t idx) {
  Coordinate coor = ToCoor(idx);
  std::string uci{};
  uci.push_back('a' + coor.col);
  uci.push_back('0' + (8 - coor.row));
  return uci;
}

bool IsOnTheBoard(const Coordinate coor) {
  return (coor.row >= 0 && coor.row < 8 && coor.col >= 0 && coor.col < 8);
}

ISquarePtr Board::Get(const Coordinate coor) const {
  if (IsOnTheBoard(coor)) {
    return squares_[ToIdx(coor)];
  } else {
    throw std::out_of_range{"Board::Get: Out of range."};
  }
}

ISquarePtr Board::Get(const std::size_t idx) const { return squares_.at(idx); }

void Board::Set(const Coordinate coor, const ISquarePtr square) {
  if (IsOnTheBoard(coor)) {
    squares_[ToIdx(coor)] = square;
  } else {
    throw std::out_of_range{"Board::Set: Out of range."};
  }
}

void Board::Set(const std::size_t idx, const ISquarePtr square) {
  squares_.at(idx) = square;
}

void Board::SwapSquares(const std::size_t a, const std::size_t b) {
  std::swap(squares_.at(a), squares_.at(b));
}

Castling State::GetCastling(const Player player) const {
  return player == Player::max ? castling_white_ : castling_black_;
}

void State::SetCastling(const Player player, const Castling castling) {
  if (player == Player::max) {
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

  return stream;
}

std::ostream& operator<<(std::ostream& stream, const State& state) {
  if (state.turn_ == Player::max) {
    stream << "White to move at ply " << state.plies_ << ":\n";
  } else {
    stream << "Black to move at ply " << state.plies_ << ":\n";
  }

  stream << state.board_;
  return stream;
}

}  // namespace Chess
