#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include "alpha_beta/minimax.h"
#include "chess/state.h"

#include <map>

namespace GameTree {

using Chess::Piece;

template <>
Evaluation evaluate(const Chess::State& state) {
  const std::map<Chess::Piece, float> piece_values{
      {Piece::e, 0.0F},    {Piece::p, -1.0F}, {Piece::r, -5.0F},
      {Piece::n, -3.0F},   {Piece::b, -3.0F}, {Piece::q, -9.0F},
      {Piece::k, -100.0F}, {Piece::P, 1.0F},  {Piece::R, 5.0F},
      {Piece::N, 3.0F},    {Piece::B, 3.0F},  {Piece::Q, 9.0F},
      {Piece::K, 100.0F},
  };

  float evaluation = 0.0F;
  for (const auto& row : state.board_.data_) {
    for (const Piece piece : row) {
      evaluation += piece_values.at(piece);
    }
  }

  return evaluation;
}

}  // namespace GameTree

// todo: Test for checkmate

#endif
