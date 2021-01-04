#ifndef SEACH_TRAVERSE_ALL_LEAVES_H
#define SEACH_TRAVERSE_ALL_LEAVES_H

#include "bitboard/move_stack.h"
#include "bitboard/position.h"

#include <type_traits>

namespace Chess
{

struct Statistic
{
    long long number_of_evaluations;
};

template <typename GenerateBehavior>
std::enable_if_t<GenerateBehavior::not_defined, const MoveStack::iterator> GenerateMoves(const Position&,
                                                                                         MoveStack::iterator);

/// @brief A search without pruning that visits all leaf nodes.
///
/// Used for debugging and benchmarking move generation.
template <typename GenerateBehavior>
void TraverseAllLeaves(const uint8_t depth,
                       Position& position,
                       const MoveStack::iterator& end_iterator_before_move_generation,
                       Statistic& stats)
{
    if (depth == 0)
    {
        stats.number_of_evaluations++;
        return;
    }

    const MoveStack::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    for (MoveStack::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation;
         move_iterator++)
    {
        const Bitboard saved_extras = position.MakeMove(*move_iterator);
        if (!position.KingIsInCheck(position.defending_side_))
        {
            TraverseAllLeaves<GenerateBehavior>(depth - 1, position, end_iterator_after_move_generation, stats);
        }
        position.UnmakeMove(*move_iterator, saved_extras);
    }
    return;
}

}  // namespace Chess

#endif
