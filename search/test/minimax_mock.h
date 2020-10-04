#ifndef SEACH_TEST_MINIMAX_MOCK_H
#define SEACH_TEST_MINIMAX_MOCK_H

#include "bitboard/move_generation.h"
#include "board/evaluation.h"
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

    const bool game_is_over = end_iterator_before_move_generation == end_iterator_after_move_generation;

    if (game_is_over)
    {
        return position.WhiteToMove() ? MIN_EVAL : MAX_EVAL;
    }

    if (position.WhiteToMove())
    {
        Evaluation max_eval{MIN_EVAL};
        for (MoveList::iterator child_move_iterator = end_iterator_before_move_generation;
             child_move_iterator != end_iterator_after_move_generation;
             child_move_iterator++)
        {
            position.MakeMove(*child_move_iterator);
            Evaluation eval = minimax<SearchBehaviour, GenerateBehavior, EvaluateBehavior>(
                depth - 1, position, end_iterator_after_move_generation);
            position.UnmakeMove(*child_move_iterator);
            max_eval = eval > max_eval ? eval : max_eval;
        }
        return max_eval;
    }
    else
    {
        Evaluation min_eval{MAX_EVAL};
        for (MoveList::iterator child_move_iterator = end_iterator_before_move_generation;
             child_move_iterator != end_iterator_after_move_generation;
             child_move_iterator++)
        {
            position.MakeMove(*child_move_iterator);
            Evaluation eval = minimax<SearchBehaviour, GenerateBehavior, EvaluateBehavior>(
                depth - 1, position, end_iterator_after_move_generation);
            min_eval = eval < min_eval ? eval : min_eval;
        }
        return min_eval;
    }
}

}  // namespace Chess

#endif
