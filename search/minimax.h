#ifndef SEARCH_MINIMAX_H
#define SEARCH_MINIMAX_H

#include "search/evaluation.h"

#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Chess {

template <typename Game>
class Node {
 public:
  Node(Game&& state) : state_(std::move(state)) {}

  std::vector<std::unique_ptr<Node>> children_{};
  Game state_{};
};

template <typename Game>
Evaluation evaluate(const Game& state);

template <typename Game>
void populate(Node<Game>& node, const int depth);

template <typename Game>
Evaluation minimax(const Node<Game>& node, const uint8_t depth,
                   const Player turn, Evaluation alpha, Evaluation beta) {
  if ((depth == 0) || !node.children_.size()) {
    return evaluate(node.state_);

  } else if (turn == Player::max) {
    Evaluation max_eval{MIN_EVAL};
    for (const std::unique_ptr<Node<Game>>& child : node.children_) {
      Evaluation eval =
          minimax<Game>(*child, depth - 1, Player::min, alpha, beta);
      max_eval = eval > max_eval ? eval : max_eval;
      alpha = eval > alpha ? eval : alpha;
      if (max_eval > beta) {
        break;
      }
    }
    return max_eval;

  } else {
    Evaluation min_eval{MAX_EVAL};
    for (const std::unique_ptr<Node<Game>>& child : node.children_) {
      Evaluation eval =
          minimax<Game>(*child, depth - 1, Player::max, alpha, beta);
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
