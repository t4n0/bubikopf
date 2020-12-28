#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <limits>
#include <tuple>
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
std::tuple<Bitmove, Evaluation> FindBestMove(const uint8_t depth,
                                             Position& position,
                                             const MoveList::iterator& end_iterator_before_move_generation,
                                             const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                                             const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintPruningInfoNodeEntry<DebugBehavior>(alpha_parent, beta_parent, position);
    if (depth == 0)
    {
        const Evaluation evaluation = evaluate<EvaluateBehavior>(position);
        constexpr Bitmove null_move = 0;
        PrintEvaluationInfo<DebugBehavior>(evaluation, position);
        return std::tie(null_move, evaluation);
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    Evaluation alpha = alpha_parent;
    Evaluation beta = beta_parent;
    Bitmove best_move;

    if (position.white_to_move_)
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                Evaluation eval;
                std::tie(std::ignore, eval) = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                if (eval > alpha)
                {
                    alpha = eval;
                    best_move = *move_iterator;
                }
            }
            position.UnmakeMove(*move_iterator);

            if (alpha >= beta)
            {
                PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
                return std::tie(best_move, alpha);
            }
        }
        PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
        return std::tie(best_move, alpha);
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
                Evaluation eval;
                std::tie(std::ignore, eval) = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                if (eval < beta)
                {
                    beta = eval;
                    best_move = *move_iterator;
                }
            }
            position.UnmakeMove(*move_iterator);

            if (beta <= alpha)
            {
                PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
                return std::tie(best_move, beta);
            }
        }
        PrintPruningInfoNodeExit<DebugBehavior>(alpha, beta, position);
        return std::tie(best_move, beta);
    }
}

}  // namespace Chess

#endif
