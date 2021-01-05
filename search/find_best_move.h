#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/fen_conversion.h"
#include "bitboard/move_stack.h"
#include "bitboard/position.h"
#include "bitboard/uci_conversion.h"

#include <iostream>
#include <limits>
#include <tuple>
#include <type_traits>

namespace Chess
{

template <typename EvaluateBehavior>
std::enable_if_t<EvaluateBehavior::not_defined, Evaluation> evaluate(const Position&);

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
void PrintNodeEntry(const Position& position, const int depth)
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
        std::cout << "returning to depth " << (depth + 1) << '\n' << std::endl;
    }
    std::ignore = depth;  // Resolve warning if debugging disabled.
}

/// @brief A negamax search using alpha/beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
std::tuple<Bitmove, Evaluation> FindBestMove(Position& position,
                                             const int depth,
                                             const MoveStack::iterator& end_iterator_before_move_generation,
                                             const Evaluation negamax_sign,
                                             const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                                             const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntry<DebugBehavior>(position, depth);
    if (depth == 0)
    {
        const Evaluation evaluation = evaluate<EvaluateBehavior>(position);
        PrintEvaluation<DebugBehavior>(evaluation);
        return {kBitNullMove, evaluation * negamax_sign};
    }

    const MoveStack::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    Evaluation alpha = alpha_parent;
    Bitmove best_move{};
    bool is_terminal_node = true;

    for (MoveStack::iterator move_iterator = end_iterator_before_move_generation;
         move_iterator != end_iterator_after_move_generation;
         move_iterator++)
    {
        const Bitboard saved_extras = position.MakeMove(*move_iterator);
        if (!position.KingIsInCheck(position.defending_side_))
        {
            PrintMove<DebugBehavior>(*move_iterator);
            is_terminal_node = false;
            Evaluation eval;
            std::tie(std::ignore, eval) = FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(
                position, depth - 1, end_iterator_after_move_generation, -negamax_sign, -beta_parent, -alpha);
            eval *= Evaluation{-1};
            if (eval > alpha)
            {
                alpha = eval;
                best_move = *move_iterator;
            }
        }
        position.UnmakeMove(*move_iterator, saved_extras);

        if (alpha >= beta_parent)
        {
            PrintPruning<DebugBehavior>(alpha, beta_parent);
            break;
        }
    }

    if (is_terminal_node)
    {
        Evaluation game_result = position.KingIsInCheck(position.attacking_side_) ? Evaluation{1000} : Evaluation{0};
        if (position.white_to_move_)
        {
            game_result *= Evaluation{-1};
        }
        PrintEvaluation<DebugBehavior>(game_result);
        PrintNodeExit<DebugBehavior>(depth);
        return {kBitNullMove, game_result * negamax_sign};
    }

    PrintNodeExit<DebugBehavior>(depth);
    return {best_move, alpha};
}

}  // namespace Chess

#endif
