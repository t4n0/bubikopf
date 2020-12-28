#ifndef SEACH_TRAVERSE_ALL_LEAVES_H
#define SEACH_TRAVERSE_ALL_LEAVES_H

#include <type_traits>

#include "bitboard/move_list.h"
#include "bitboard/position.h"

namespace Chess {

struct Statistic {
  static long long number_of_evaluations;
};

template <typename EvaluateBehavior>
std::enable_if_t<EvaluateBehavior::not_defined, Evaluation> evaluate(
    const Position&);

template <typename GenerateBehavior>
std::enable_if_t<GenerateBehavior::not_defined, const MoveList::iterator>
GenerateMoves(const Position&, MoveList::iterator);

/// @brief A search without pruning that visits all leaf nodes.
///
/// Used for debugging and benchmarks.
template <typename GenerateBehavior, typename EvaluateBehavior>
Evaluation TraverseAllLeaves(
    const uint8_t depth, Position& position,
    const MoveList::iterator& end_iterator_before_move_generation) {
  if (depth == 0) {
    Statistic::number_of_evaluations++;
    return evaluate<EvaluateBehavior>(position);
  }

  const MoveList::iterator end_iterator_after_move_generation =
      GenerateMoves<GenerateBehavior>(position,
                                      end_iterator_before_move_generation);

  if (position.white_to_move_) {
    Evaluation max_eval{std::numeric_limits<Evaluation>::lowest()};
    for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation; move_iterator++) {
      position.MakeMove(*move_iterator);
      if (!position.DefendersKingIsInCheck()) {
        Evaluation eval = TraverseAllLeaves<GenerateBehavior, EvaluateBehavior>(
            depth - 1, position, end_iterator_after_move_generation);
        max_eval = eval > max_eval ? eval : max_eval;
      }
      position.UnmakeMove(*move_iterator);
    }
    return max_eval;
  } else {
    Evaluation min_eval{std::numeric_limits<Evaluation>::max()};
    for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation; move_iterator++) {
      position.MakeMove(*move_iterator);
      if (!position.DefendersKingIsInCheck()) {
        Evaluation eval = TraverseAllLeaves<GenerateBehavior, EvaluateBehavior>(
            depth - 1, position, end_iterator_after_move_generation);
        min_eval = eval < min_eval ? eval : min_eval;
      }

      position.UnmakeMove(*move_iterator);
    }
    return min_eval;
  }
}

}  // namespace Chess

#endif
