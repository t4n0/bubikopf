#include "board/position.h"
#include "board/pieces.h"

#include <algorithm>
#include <memory>

namespace Chess {

Position::Position() {
  for (auto& square : board_.squares_) {
    square = Empty::Make();
  }
}

std::vector<Position> Position::FindPlies() const {
  std::vector<Position> collected_plies{};
  for (std::size_t idx{0}; idx < board_.squares_.size(); ++idx) {
    const ISquare& square = *board_.squares_.at(idx);
    std::vector<Position> new_plies = square.FindPlies(idx, *this);

    collected_plies.insert(collected_plies.end(),
                           std::make_move_iterator(new_plies.begin()),
                           std::make_move_iterator(new_plies.end()));
  }

  return collected_plies;
}

Player Position::GetTurn() const {
  return plies_ % 2 ? Player::min : Player::max;
}

Castling Position::GetCastling(const Player player) const {
  return player == Player::max ? castling_white_ : castling_black_;
}

void Position::SetCastling(const Player player, const Castling castling) {
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

std::ostream& operator<<(std::ostream& stream, const Position& position) {
  if (position.GetTurn() == Player::max) {
    stream << "White to move at ply " << position.plies_ << ":\n";
  } else {
    stream << "Black to move at ply " << position.plies_ << ":\n";
  }

  stream << position.board_;
  return stream;
}

}  // namespace Chess
