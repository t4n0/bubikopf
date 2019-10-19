#include <chess/state.h>

#include <map>
#include <string>

namespace Chess {

bool operator==(const Coordinate& a, const Coordinate& b) {
  return (a.col == b.col) && (a.row == b.row);
}

bool operator!=(const Coordinate& a, const Coordinate& b) { return !(a == b); }

Piece Board::Get(const Coordinate coor) const {
  return data_.at(static_cast<std::size_t>(coor.row))
      .at(static_cast<std::size_t>(coor.col));
}

void Board::Set(const Coordinate coor, const Piece piece) {
  data_.at(static_cast<std::size_t>(coor.row))
      .at(static_cast<std::size_t>(coor.col)) = piece;
}

std::ostream& operator<<(std::ostream& stream, const Piece piece) {
  const std::map<Piece, std::string> PieceAbbreviationMap{
      {Piece::p, "p"}, {Piece::r, "r"}, {Piece::n, "n"}, {Piece::b, "b"},
      {Piece::q, "q"}, {Piece::k, "k"}, {Piece::P, "P"}, {Piece::R, "R"},
      {Piece::N, "N"}, {Piece::B, "B"}, {Piece::Q, "Q"}, {Piece::K, "K"},
      {Piece::e, "_"},
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

}  // namespace Chess
