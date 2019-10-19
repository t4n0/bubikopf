#include <chess/state.h>

#include <map>

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

Piece Board::Get(const Coordinate coor) const {
  if (IsOnTheBoard(coor)) {
    return data_[static_cast<std::size_t>(coor.row)]
                [static_cast<std::size_t>(coor.col)];
  } else {
    throw std::out_of_range{"Board::Get: Out of range."};
  }
}

void Board::Set(const Coordinate coor, const Piece piece) {
  if (IsOnTheBoard(coor)) {
    data_[static_cast<std::size_t>(coor.row)]
         [static_cast<std::size_t>(coor.col)] = piece;
  } else {
    throw std::out_of_range{"Board::Set: Out of range."};
  }
}

std::ostream& operator<<(std::ostream& stream, const Piece piece) {
  const std::map<Piece, char> PieceAbbreviationMap{
      {Piece::p, 'p'}, {Piece::r, 'r'}, {Piece::n, 'n'}, {Piece::b, 'b'},
      {Piece::q, 'q'}, {Piece::k, 'k'}, {Piece::P, 'P'}, {Piece::R, 'R'},
      {Piece::N, 'N'}, {Piece::B, 'B'}, {Piece::Q, 'Q'}, {Piece::K, 'K'},
      {Piece::e, '_'},
  };
  stream << PieceAbbreviationMap.at(piece);

  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Board& board) {
  for (const auto& row : board.data_) {
    for (const auto piece : row) {
      stream << piece << " ";
    }
    stream << '\n';
  }
  stream << '\n';

  return stream;
}

std::ostream& operator<<(std::ostream& stream, const State& state) {
  stream << state.board_;
  return stream;
}

bool IsAPieceOfSide(const Piece piece, const GameTree::Player side) {
  if (piece == Piece::e) {
    return false;
  } else {
    const bool piece_is_black{piece >= Piece::p && piece <= Piece::k};
    const bool piece_is_white{piece >= Piece::P && piece <= Piece::K};

    return (piece_is_black && side == GameTree::Player::min) ||
           (piece_is_white && side == GameTree::Player::max);
  }
}

bool IsOnTheBoard(const Coordinate coor) {
  return (coor.row >= 0 && coor.row < 8 && coor.col >= 0 && coor.col < 8);
}

}  // namespace Chess
