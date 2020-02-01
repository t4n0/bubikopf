#ifndef SEARCH_EVALUATE_H
#define SEARCH_EVALUATE_H

#include "board/state.h"
#include "search/evaluation.h"

#include <type_traits>

namespace Chess {

struct ConcreteEvaluation {
  static constexpr bool is_mock{false};
};

template <typename Behaviour = ConcreteEvaluation>
std::enable_if_t<!Behaviour::is_mock, Evaluation> evaluate(const State& state) {
  float evaluation = 0.0F;
  for (const auto& square : state.board_.squares_) {
    evaluation += square->GetValue();
  }
  return evaluation;
}

}  // namespace Chess

#endif
