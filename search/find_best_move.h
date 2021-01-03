#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/fen_conversion.h"
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
    static constexpr bool debugging = false;
};

struct DebuggingEnabled
{
    static constexpr bool debugging = true;
};

template <typename Behavior>
void PrintNodeEntry(const uint8_t depth, const Position& position)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "depth " << int{depth} << '\n';
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
void PrintNodeExit(const uint8_t depth)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "returning to depth " << int{depth + 1} << '\n' << std::endl;
    }
    std::ignore = depth;  // Resolve warning if debugging disabled.
}

/// @brief A minimax search using alpha / beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
std::tuple<Bitmove, Evaluation> FindBestMove(const uint8_t depth,
                                             Position& position,
                                             const MoveList::iterator& end_iterator_before_move_generation,
                                             const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                                             const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntry<DebugBehavior>(depth, position);
    if (depth == 0)
    {
        const Evaluation evaluation = evaluate<EvaluateBehavior>(position);
        constexpr Bitmove null_move = 0;
        PrintEvaluation<DebugBehavior>(evaluation);
        return {null_move, evaluation};
    }

    const MoveList::iterator end_iterator_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_iterator_before_move_generation);

    Evaluation alpha = alpha_parent;
    Evaluation beta = beta_parent;
    Bitmove best_move{};
    bool one_legal_move_found = false;

    if (position.white_to_move_)
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            const Bitboard saved_extras = position.MakeMove(*move_iterator);
            if (!position.KingIsInCheck(position.defending_side_))
            {
                PrintMove<DebugBehavior>(*move_iterator);
                one_legal_move_found = true;
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
                PrintPruning<DebugBehavior>(alpha, beta);
                PrintNodeExit<DebugBehavior>(depth);
                return {best_move, alpha};
            }
        }

        if (!one_legal_move_found)
        {
            const Evaluation game_result =
                position.KingIsInCheck(position.attacking_side_) ? Evaluation{-1000} : Evaluation{0};
            PrintEvaluation<DebugBehavior>(game_result);
            PrintNodeExit<DebugBehavior>(depth);
            return {Bitmove{}, game_result};
        }

        PrintNodeExit<DebugBehavior>(depth);
        return {best_move, alpha};
    }
    else
    {
        for (MoveList::iterator move_iterator = end_iterator_before_move_generation;
             move_iterator != end_iterator_after_move_generation;
             move_iterator++)
        {
            const Bitboard saved_extras = position.MakeMove(*move_iterator);
            if (!position.KingIsInCheck(position.defending_side_))
            {
                PrintMove<DebugBehavior>(*move_iterator);
                one_legal_move_found = true;
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
                PrintPruning<DebugBehavior>(alpha, beta);
                PrintNodeExit<DebugBehavior>(depth);
                return {best_move, beta};
            }
        }

        if (!one_legal_move_found)
        {
            const Evaluation game_result =
                position.KingIsInCheck(position.attacking_side_) ? Evaluation{1000} : Evaluation{0};
            PrintEvaluation<DebugBehavior>(game_result);
            PrintNodeExit<DebugBehavior>(depth);
            return {Bitmove{}, game_result};
        }

        PrintNodeExit<DebugBehavior>(depth);
        return {best_move, beta};
    }
}

}  // namespace Chess

#endif
