#include "board/state.h"
#include "board/pieces.h"

#include <algorithm>
#include <memory>

namespace Chess {

State::State() {
  for (auto& square : board_.squares_) {
    square = Empty::Make();
  }
}

std::vector<State> State::FindPlies() const {
  std::vector<State> collected_plies{};
  for (std::size_t idx{0}; idx < board_.squares_.size(); ++idx) {
    const ISquare& square = *board_.squares_.at(idx);
    std::vector<State> new_plies = square.FindPlies(idx, *this);

    collected_plies.insert(collected_plies.end(),
                           std::make_move_iterator(new_plies.begin()),
                           std::make_move_iterator(new_plies.end()));
  }

  return collected_plies;
}

Player State::GetTurn() const { return plies_ % 2 ? Player::min : Player::max; }

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
