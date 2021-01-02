#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <iostream>
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
std::enable_if_t<DebugBehavior::disabled, void> PrintNodeEntryInfo(const uint8_t /*unused*/)
{
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintNodeExitInfo(const uint8_t /*unused*/)
{
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintEvaluationInfo(const Evaluation /*unused*/)
{
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::disabled, void> PrintMoveInfo(const Bitmove /*unused*/)
{
}

struct DebuggingEnabled
{
    static constexpr bool enabled = true;
};

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintNodeEntryInfo(const uint8_t depth)
{
    std::cout << "entering new node at depth " << int{depth} << std::endl;
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintNodeExitInfo(const uint8_t depth)
{
    std::cout << "leaving node at depth " << int{depth} << std::endl;
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintEvaluationInfo(const Evaluation evaluation)
{
    std::cout << "node evaluates to " << evaluation << std::endl;
}

template <typename DebugBehavior>
std::enable_if_t<DebugBehavior::enabled, void> PrintMoveInfo(const Bitmove move)
{
    std::cout << "investigating move " << ToUciString(move) << std::endl;
}

/// @brief A minimax search using alpha / beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
std::tuple<Bitmove, Evaluation> FindBestMove(const uint8_t depth,
                                             Position& position,
                                             const MoveList::iterator& end_iterator_before_move_generation,
                                             const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                                             const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntryInfo<DebugBehavior>(depth);
    if (depth == 0)
    {
        const Evaluation evaluation = evaluate<EvaluateBehavior>(position);
        constexpr Bitmove null_move = 0;
        PrintEvaluationInfo<DebugBehavior>(evaluation);
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
            const Bitboard saved_extras = position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                PrintMoveInfo<DebugBehavior>(*move_iterator);
                Evaluation eval;
                std::tie(std::ignore, eval) = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                if (eval > alpha)
                {
                    alpha = eval;
                    best_move = *move_iterator;
                }
            }
            position.UnmakeMove(*move_iterator, saved_extras);

            if (alpha >= beta)
            {
                PrintNodeExitInfo<DebugBehavior>(depth);
                return std::tie(best_move, alpha);
            }
        }
        PrintNodeExitInfo<DebugBehavior>(depth);
        return std::tie(best_move, alpha);
    }
    else
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            const Bitboard saved_extras = position.MakeMove(*move_iterator);
            if (!position.DefendersKingIsInCheck())
            {
                PrintMoveInfo<DebugBehavior>(*move_iterator);
                Evaluation eval;
                std::tie(std::ignore, eval) = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                    depth - 1, position, end_iterator_after_move_generation, alpha, beta);
                if (eval < beta)
                {
                    beta = eval;
                    best_move = *move_iterator;
                }
            }
            position.UnmakeMove(*move_iterator, saved_extras);

            if (beta <= alpha)
            {
                PrintNodeExitInfo<DebugBehavior>(depth);
                return std::tie(best_move, beta);
            }
        }
        PrintNodeExitInfo<DebugBehavior>(depth);
        return std::tie(best_move, beta);
    }
}

}  // namespace Chess

#endif
