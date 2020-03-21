#ifndef SEARCH_MINIMAX_H
#define SEARCH_MINIMAX_H

#include "search/evaluate.h"
#include "search/node.h"

#include <cstdint>
#include <memory>

namespace Chess {

namespace {
bool GameIsOver(const Node& node) { return node.children_.size() == 0; }
}  // namespace

template <typename Behaviour = Production>
Evaluation minimax(const Node& node, const uint8_t depth, const Player turn,
                   Evaluation alpha, Evaluation beta) {
  if ((depth == 0) || GameIsOver(node)) {
    return evaluate<Behaviour>(node.state_);

  } else if (turn == Player::max) {
    Evaluation max_eval{MIN_EVAL};
    for (const std::unique_ptr<Node>& child : node.children_) {
      Evaluation eval =
          minimax<Behaviour>(*child, depth - 1, Player::min, alpha, beta);
      max_eval = eval > max_eval ? eval : max_eval;
      alpha = eval > alpha ? eval : alpha;
      if (max_eval > beta) {
        break;
      }
    }
    return max_eval;

  } else {
    Evaluation min_eval{MAX_EVAL};
    for (const std::unique_ptr<Node>& child : node.children_) {
      Evaluation eval =
          minimax<Behaviour>(*child, depth - 1, Player::max, alpha, beta);
      min_eval = eval < min_eval ? eval : min_eval;
      beta = eval < beta ? eval : beta;
      if (min_eval < alpha) {
        break;
      }
    }
    return min_eval;
  }
}

}  // namespace Chess

#endif
