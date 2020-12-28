#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <type_traits>

namespace Chess
{

template <typename EvaluateBehavior>
std::enable_if_t<EvaluateBehavior::not_defined, Evaluation> evaluate(const Position&);

template <typename GenerateBehavior>
std::enable_if_t<GenerateBehavior::not_defined, const MoveList::iterator> GenerateMoves(const Position&,
                                                                                        MoveList::iterator);

struct DebuggingDisabled
{
    static constexpr bool disabled = true;
};

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintPruningInfoNodeEntry(const Evaluation /*unused*/,
                                                                          const Evaluation /*unused*/,
                                                                          const Position& /*unused*/)
{
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintPruningInfoNodeExit(const Evaluation /*unused*/,
                                                                         const Evaluation /*unused*/,
                                                                         const Position& /*unused*/)
{
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintEvaluationInfo(const Evaluation /*unused*/,
                                                                    const Position& /*unused*/)
{
}

/// @brief A minimax search using alpha / beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
Evaluation FindBestMove(const uint8_t depth,
                        Position& position,
                        const MoveList::iterator& end_iterator_before_move_generation,
                        const Evaluation alpha_parent,
                        const Evaluation beta_parent)
{
    PrintPruningInfoNodeEntry<DebugBehavior>(alpha_parent, beta_parent, position);
    if (depth == 0)
    {
        const Evaluation evaluation = evaluate<EvaluateBehavior>(position);
        PrintEvaluationInfo<DebugBehavior>(evaluation, position);
        return evaluation;
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    Evaluation alpha = alpha_parent;
    Evaluation beta = beta_parent;

    if (position.white_to_move_)
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                Evaluation eval = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                alpha = std::max(eval, alpha);
            }
            position.UnmakeMove(*move_iterator);

            if (alpha >= beta)
            {
                PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
                return alpha;
            }
        }
        PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
        return alpha;
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
                Evaluation eval = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                beta = std::min(eval, beta);
            }
            position.UnmakeMove(*move_iterator);

            if (beta <= alpha)
            {
                PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
                return beta;
            }
        }
        PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
        return beta;
    }
}

}  // namespace Chess

#endif
