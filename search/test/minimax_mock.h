#ifndef SEACH_TEST_MINIMAX_MOCK_H
#define SEACH_TEST_MINIMAX_MOCK_H

#include "bitboard/move_generation.h"
#include "search/test/evaluate_mock.h"

#include <type_traits>

namespace Chess
{

struct SearchAllBranchesWithoutPruning
{
    static constexpr bool search_all_branches_without_pruning = true;
};

template <typename SearchBehaviour, typename GenerateBehavior, typename EvaluateBehavior>
std::enable_if_t<SearchBehaviour::search_all_branches_without_pruning, Evaluation> minimax(
    const uint8_t depth,
    PositionWithBitboards& position,
    const MoveList::iterator& end_iterator_before_move_generation)
{
    if (depth == 0)
    {
        return evaluate<EvaluateBehavior>(position);
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    if (position.WhiteToMove())
    {
        Evaluation max_eval{std::numeric_limits<Evaluation>::lowest()};
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                Evaluation eval = minimax<SearchBehaviour, GenerateBehavior, EvaluateBehavior>(
                    depth - 1, position, end_iterator_after_move_generation);
                max_eval = eval > max_eval ? eval : max_eval;
            }
            position.UnmakeMove(*move_iterator);
        }
        return max_eval;
    }
    else
    {
        Evaluation min_eval{std::numeric_limits<Evaluation>::max()};
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                Evaluation eval = minimax<SearchBehaviour, GenerateBehavior, EvaluateBehavior>(
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
