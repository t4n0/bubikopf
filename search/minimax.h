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

struct SearchAllBranchesWithoutPruning {
  static constexpr bool search_all_branches_without_pruning = true;
};

template <typename SearchBehaviour = SearchAllBranchesWithoutPruning,
          typename GenerateMoveBehavior = GenerateTwoMovesWithUniqueDebugId,
          typename EvaluateBehavior = EvaluteToZero>
std::enable_if_t<SearchBehaviour::search_all_branches_without_pruning,
                 Evaluation>
minimax(const uint8_t depth, PositionWithBitboards& position,
        const move_list_t::iterator& end_iterator_before_move_generation) {
  if (depth == 0) {
    return evaluate<EvaluateBehavior>(position);
  }

  const move_list_t::iterator end_iterator_after_move_generation =
      GenerateMoves<GenerateMoveBehavior>(position,
                                          end_iterator_before_move_generation);

  const bool game_is_over =
      end_iterator_before_move_generation == end_iterator_after_move_generation;

  if (game_is_over) {
    return position.WhiteToMove() ? MIN_EVAL : MAX_EVAL;
  }

  else if (position.WhiteToMove()) {
    Evaluation max_eval{MIN_EVAL};
    for (move_list_t::iterator child_move_iterator =
             end_iterator_before_move_generation;
         child_move_iterator != end_iterator_after_move_generation;
         child_move_iterator++) {
      position.MakeMove(*child_move_iterator);
      Evaluation eval = minimax<SearchBehaviour>(
          depth - 1, position, end_iterator_after_move_generation);
      position.UnmakeMove(*child_move_iterator);
      max_eval = eval > max_eval ? eval : max_eval;
    }
    return max_eval;
  } else {
    Evaluation min_eval{MAX_EVAL};
    for (move_list_t::iterator child_move_iterator =
             end_iterator_before_move_generation;
         child_move_iterator != end_iterator_after_move_generation;
         child_move_iterator++) {
      position.MakeMove(*child_move_iterator);
      Evaluation eval = minimax<SearchBehaviour>(
          depth - 1, position, end_iterator_after_move_generation);
      min_eval = eval < min_eval ? eval : min_eval;
    }
    return min_eval;
  }
}

}  // namespace Chess

#endif
