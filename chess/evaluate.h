#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include "alpha_beta/minimax.h"
#include "chess/state.h"

#include <map>

namespace AlphaBeta {

template <>
Evaluation evaluate(const Chess::State& state) {
  float evaluation = 0.0F;
  for (const auto& square : state.board_.squares_) {
    evaluation += square->GetValue();
  }
  return evaluation;
}

}  // namespace AlphaBeta

#endif
