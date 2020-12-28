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
                       Statistic& stats,
                       const MoveList::iterator& end_iterator_before_move_generation)
{
    if (depth == 0)
    {
        stats.number_of_evaluations++;
        return evaluate<EvaluateBehavior>(position);
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    if (position.white_to_move_)
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                TraverseAllLeaves<GenerateBehavior, EvaluateBehavior>(
                    depth - 1, position, end_iterator_after_move_generation);
            }
            position.UnmakeMove(*move_iterator);
        }
        return;
    }
    else
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                Evaluation eval = TraverseAllLeaves<GenerateBehavior, EvaluateBehavior>(
                    depth - 1, position, end_iterator_after_move_generation);
            }

            position.UnmakeMove(*move_iterator);
        }
        return;
    }
}

}  // namespace Chess

#endif
