#include "chess/state.h"

namespace Chess {

std::ostream& operator<<(std::ostream& stream, const Square& square) {
  return square.print(stream);
}

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

bool IsOnTheBoard(const Coordinate coor) {
  return (coor.row >= 0 && coor.row < 8 && coor.col >= 0 && coor.col < 8);
}

std::size_t ToIdx(const Coordinate coor) {
  return static_cast<std::size_t>(coor.col + 8 * coor.row);
}

const SquarePtr& Board::Get(const Coordinate coor) const {
  if (IsOnTheBoard(coor)) {
    return squares_[ToIdx(coor)];
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

Square::~Square() {}

float Empty::GetValue() const { return 0.0F; }
float Pawn::GetValue() const {
  return side_ == GameTree::Player::max ? 1.0F : -1.0F;
}
float Knight::GetValue() const {
  return side_ == GameTree::Player::max ? 3.0F : -3.0F;
}
float Bishop::GetValue() const {
  return side_ == GameTree::Player::max ? 3.0F : -3.0F;
}
float Rook::GetValue() const {
  return side_ == GameTree::Player::max ? 5.0F : -5.0F;
}
float Queen::GetValue() const {
  return side_ == GameTree::Player::max ? 9.0F : -9.0F;
}
float King::GetValue() const {
  return side_ == GameTree::Player::max ? 100.0F : -100.0F;
}

bool Empty::IsOfSide(const GameTree::Player& /*unused*/) { return false; }
bool Pawn::IsOfSide(const GameTree::Player& side) { return side_ == side; }
bool Knight::IsOfSide(const GameTree::Player& side) { return side_ == side; }
bool Bishop::IsOfSide(const GameTree::Player& side) { return side_ == side; }
bool Rook::IsOfSide(const GameTree::Player& side) { return side_ == side; }
bool Queen::IsOfSide(const GameTree::Player& side) { return side_ == side; }
bool King::IsOfSide(const GameTree::Player& side) { return side_ == side; }

bool Empty::IsEmpty() const { return true; }
bool Pawn::IsEmpty() const { return false; }
bool Knight::IsEmpty() const { return false; }
bool Bishop::IsEmpty() const { return false; }
bool Rook::IsEmpty() const { return false; }
bool Queen::IsEmpty() const { return false; }
bool King::IsEmpty() const { return false; }

std::ostream& Empty::print(std::ostream& stream) const {
  stream << "_ ";
  return stream;
}

std::ostream& Pawn::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "P ";
  } else {
    stream << "p ";
  }
  return stream;
}

std::ostream& Knight::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "N ";
  } else {
    stream << "n ";
  }
  return stream;
}

std::ostream& Bishop::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "B ";
  } else {
    stream << "b ";
  }
  return stream;
}

std::ostream& Rook::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "R ";
  } else {
    stream << "r ";
  }
  return stream;
}

std::ostream& Queen::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "Q ";
  } else {
    stream << "q ";
  }
  return stream;
}

std::ostream& King::print(std::ostream& stream) const {
  if (side_ == GameTree::Player::max) {
    stream << "K ";
  } else {
    stream << "k ";
  }
  return stream;
}

Board::Board() {
  for (auto& square : squares_) {
    square = std::make_unique<Empty>();
  }
}

}  // namespace Chess
