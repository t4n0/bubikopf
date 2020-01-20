#ifndef BOARD_UTILITIES_H
#define BOARD_UTILITIES_H

#include "board/piece.h"
#include "board/state.h"

namespace Chess {

inline const SquareBehaviourPool POOL{};

inline State SetUpEmptyBoard() {
  State state{POOL};
  for (std::size_t idx{0}; idx < state.board_.squares_.size(); idx++) {
    state.board_.Set(idx, POOL.GetEmpty());
  }

  return state;
}

inline State SetUpBoard() {
  State state{SetUpEmptyBoard()};
  // Pawns
  for (std::int8_t col{0}; col < 8; col++) {
    state.board_.Set({col, 1}, POOL.GetPawn(AlphaBeta::Player::min));
    state.board_.Set({col, 6}, POOL.GetPawn(AlphaBeta::Player::max));
  }
  // Rooks
  state.board_.Set({0, 0}, POOL.GetRook(AlphaBeta::Player::min));
  state.board_.Set({7, 0}, POOL.GetRook(AlphaBeta::Player::min));
  state.board_.Set({0, 7}, POOL.GetRook(AlphaBeta::Player::max));
  state.board_.Set({7, 7}, POOL.GetRook(AlphaBeta::Player::max));
  // Knights
  state.board_.Set({1, 0}, POOL.GetKnight(AlphaBeta::Player::min));
  state.board_.Set({6, 0}, POOL.GetKnight(AlphaBeta::Player::min));
  state.board_.Set({1, 7}, POOL.GetKnight(AlphaBeta::Player::max));
  state.board_.Set({6, 7}, POOL.GetKnight(AlphaBeta::Player::max));
  // Bishops
  state.board_.Set({2, 0}, POOL.GetBishop(AlphaBeta::Player::min));
  state.board_.Set({5, 0}, POOL.GetBishop(AlphaBeta::Player::min));
  state.board_.Set({2, 7}, POOL.GetBishop(AlphaBeta::Player::max));
  state.board_.Set({5, 7}, POOL.GetBishop(AlphaBeta::Player::max));
  // Queens
  state.board_.Set({3, 0}, POOL.GetQueen(AlphaBeta::Player::min));
  state.board_.Set({3, 7}, POOL.GetQueen(AlphaBeta::Player::max));
  // Kings
  state.board_.Set({4, 0}, POOL.GetKing(AlphaBeta::Player::min));
  state.board_.Set({4, 7}, POOL.GetKing(AlphaBeta::Player::max));

  return state;
}

}  // namespace Chess

#endif
