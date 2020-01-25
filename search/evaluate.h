#ifndef SEARCH_EVALUATE_H
#define SEARCH_EVALUATE_H

#include "board/state.h"
#include "search/minimax.h"

namespace Chess {

template <>
inline Evaluation evaluate(const State& state) {
  float evaluation = 0.0F;
  for (const auto& square : state.board_.squares_) {
    evaluation += square->GetValue();
  }
  return evaluation;
}

}  // namespace Chess

#endif
