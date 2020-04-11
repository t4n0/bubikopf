#ifndef EVALUATE_EVALUATE_H
#define EVALUATE_EVALUATE_H

#include "board/position.h"
#include "evaluate/evaluation.h"

#include <type_traits>

namespace Chess {

struct Production {
  static constexpr bool is_concrete{true};
};

template <typename Behaviour = Production>
std::enable_if_t<Behaviour::is_concrete, Evaluation> evaluate(
    const Position& position) {
  float evaluation = 0.0F;
  for (const auto& square : position.board_.squares_) {
    evaluation += square->GetValue();
  }
  return evaluation;
}

}  // namespace Chess

#endif
