#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/fen_conversion.h"
#include "bitboard/move_stack.h"
#include "bitboard/position.h"
#include "bitboard/uci_conversion.h"
#include "search/abort_condition.h"
#include "search/material_difference_comparison.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>

namespace Chess
{

template <typename EvaluateBehavior>
std::enable_if_t<EvaluateBehavior::not_defined, Evaluation> Evaluate(const Position&);

template <typename GenerateBehavior>
std::enable_if_t<GenerateBehavior::not_defined, const MoveStack::iterator> GenerateMoves(const Position&,
                                                                                         MoveStack::iterator);

struct DebuggingDisabled
{
    static constexpr bool debugging = false;
};

struct DebuggingEnabled
{
    static constexpr bool debugging = true;
};

template <typename Behavior>
void PrintNodeEntry(const Position& position, const std::size_t depth)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "depth " << depth << '\n';
        PrettyPrintFen(FenFromPosition(position));
    }
    std::ignore = depth;  // Resolve warning if debugging disabled.
    std::ignore = position;
}

template <typename Behavior>
void PrintEvaluation(const Evaluation evaluation)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "= " << evaluation << '\n' << std::endl;
    }
    std::ignore = evaluation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintMove(const Bitmove move)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << ToUciString(move) << '\n' << std::endl;
    }
    std::ignore = move;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintPruning(const Evaluation alpha, const Evaluation beta)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "pruning due: alpha = " << alpha << ", beta = " << beta << '\n' << std::endl;
    }
    std::ignore = alpha;  // Resolve warning if debugging disabled.
    std::ignore = beta;
}

template <typename Behavior>
void PrintNodeExit(const int depth)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "returning to depth " << (depth - 1) << '\n' << std::endl;
    }
    std::ignore = depth;  // Resolve warning if debugging disabled.
}

/// @brief A negamax search using alpha/beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
std::tuple<Bitmove, Evaluation> FindBestMove(Position& position,
                                             const MoveStack::iterator end_iterator_before_move_generation,
                                             const Evaluation negamax_sign,
                                             const AbortCondition& abort_condition,
                                             const std::size_t current_depth = 0,
                                             const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                                             const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntry<DebugBehavior>(position, current_depth);
    if (current_depth == abort_condition.full_search_depth)
    {
        const Evaluation evaluation = Evaluate<EvaluateBehavior>(position);
        PrintEvaluation<DebugBehavior>(evaluation);
        return {kBitNullMove, evaluation * negamax_sign};
    }

    const MoveStack::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);
    std::sort(end_iterator_before_move_generation, end_iterator_after_move_generation, IsMaterialDifferenceGreater);

    Evaluation alpha = alpha_parent;
    Bitmove best_move{};
    bool is_terminal_node = true;

    for (MoveStack::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation;
         move_iterator++)
    {
        const Bitmove current_move = *move_iterator;
        const Bitboard saved_extras = position.MakeMove(current_move);
        if (!position.IsKingInCheck(position.defending_side_))
        {
            PrintMove<DebugBehavior>(current_move);
            is_terminal_node = false;
            Evaluation eval;
            std::tie(std::ignore, eval) =
                FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(position,
                                                                                end_iterator_after_move_generation,
                                                                                -negamax_sign,
                                                                                abort_condition,
                                                                                current_depth + 1,
                                                                                -beta_parent,
                                                                                -alpha);
            eval *= Evaluation{-1};
            if (eval > alpha)
            {
                alpha = eval;
                best_move = current_move;
            }
        }
        position.UnmakeMove(current_move, saved_extras);

        if (alpha >= beta_parent)
        {
            PrintPruning<DebugBehavior>(alpha, beta_parent);
            break;
        }
    }

    if (is_terminal_node)
    {
        constexpr Evaluation draw = Evaluation{0};
        const Evaluation opponent_win = position.white_to_move_ ? Evaluation{-1000} : Evaluation{1000};
        const Evaluation game_result = position.IsKingInCheck(position.attacking_side_) ? opponent_win : draw;
        PrintEvaluation<DebugBehavior>(game_result);
        PrintNodeExit<DebugBehavior>(current_depth);
        return {kBitNullMove, game_result * negamax_sign};
    }

    PrintNodeExit<DebugBehavior>(current_depth);
    return {best_move, alpha};
}

}  // namespace Chess

#endif
