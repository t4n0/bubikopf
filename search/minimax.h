#ifndef SEARCH_MINIMAX_H
#define SEARCH_MINIMAX_H

#include "bitboard/move_generation.h"
#include "evaluate/evaluate.h"
#include "search/node.h"

#include <algorithm>
#include <cstdint>
#include <memory>

namespace Chess {

namespace {
bool GameIsOver(const Node& node) { return node.children_.size() == 0; }
}  // namespace

template <typename Behaviour = Production>
Evaluation minimax(Node& node, const uint8_t depth,
                   const Evaluation alpha_parent,
                   const Evaluation beta_parent) {
  if ((depth == 0) || GameIsOver(node)) {
    node.position_.evaluation_ = evaluate<Behaviour>(node.position_);
    return *node.position_.evaluation_;

  } else if (node.position_.GetTurn() == Player::max) {
    Evaluation alpha{MIN_EVAL};
    for (const std::unique_ptr<Node>& child : node.children_) {
      Evaluation eval =
          minimax<Behaviour>(*child, depth - 1, alpha, beta_parent);
      alpha = eval > alpha ? eval : alpha;
      if (alpha > beta_parent) {
        break;
      }
    }
    node.position_.evaluation_ = alpha;
    return alpha;

  } else {
    Evaluation beta{MAX_EVAL};
    for (const std::unique_ptr<Node>& child : node.children_) {
      Evaluation eval =
          minimax<Behaviour>(*child, depth - 1, alpha_parent, beta);
      beta = eval < beta ? eval : beta;
      if (alpha_parent > beta) {
        break;
      }
    }
    node.position_.evaluation_ = beta;
    return beta;
  }
}

}  // namespace Chess

#endif
