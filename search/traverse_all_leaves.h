#ifndef SEACH_TRAVERSE_ALL_LEAVES_H
#define SEACH_TRAVERSE_ALL_LEAVES_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <type_traits>

namespace Chess
{

struct Statistic
{
    long long number_of_evaluations;
};

template <typename EvaluateBehavior>
std::enable_if_t<EvaluateBehavior::not_defined, Evaluation> evaluate(const Position&);

template <typename GenerateBehavior>
std::enable_if_t<GenerateBehavior::not_defined, const MoveList::iterator> GenerateMoves(const Position&,
                                                                                        MoveList::iterator);

/// @brief A search without pruning that visits all leaf nodes.
///
/// Used for debugging and benchmarks.
template <typename GenerateBehavior, typename EvaluateBehavior>
void TraverseAllLeaves(const uint8_t depth,
                       Position& position,
                       const MoveList::iterator& end_iterator_before_move_generation,
                       Statistic& stats)
{
    if (depth == 0)
    {
        stats.number_of_evaluations++;
        return;
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation;
         move_iterator++)
    {
        position.MakeMove(*move_iterator);
        if (!position.DefendersKingIsInCheck())
        {
            TraverseAllLeaves<GenerateBehavior, EvaluateBehavior>(
                depth - 1, position, end_iterator_after_move_generation, stats);
        }
        position.UnmakeMove(*move_iterator);
    }
    return;
}

}  // namespace Chess

#endif
