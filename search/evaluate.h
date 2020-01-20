#ifndef SEARCH_EVALUATE_H
#define SEARCH_EVALUATE_H

#include "board/state.h"
#include "search/minimax.h"

namespace AlphaBeta {

template <>
inline Evaluation evaluate(const Chess::State& state) {
  float evaluation = 0.0F;
  for (const auto& square : state.board_.squares_) {
    evaluation += square->GetValue();
  }
  return evaluation;
}

}  // namespace AlphaBeta

#endif
