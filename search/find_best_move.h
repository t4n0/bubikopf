#ifndef SEARCH_FIND_BEST_MOVE_H
#define SEARCH_FIND_BEST_MOVE_H

#include "bitboard/fen_conversion.h"
#include "bitboard/move_stack.h"
#include "bitboard/position.h"
#include "bitboard/uci_conversion.h"
#include "search/abort_condition.h"
#include "search/material_difference_comparison.h"
#include "search/principal_variation.h"

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
        std::cout << "entering depth " << depth << '\n';
        PrettyPrintFen(FenFromPosition(position));
    }
    std::ignore = depth;  // Resolve warning if debugging disabled.
    std::ignore = position;
}

template <typename Behavior>
void PrintEvaluation(const Evaluation minimax_evaluation)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "= " << minimax_evaluation << '\n' << std::endl;
    }
    std::ignore = minimax_evaluation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintGeneratedMoves(const MoveStack::const_iterator begin, const MoveStack::const_iterator end)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "moves generated: ";
        std::for_each(begin, end, [](const auto move) { std::cout << ToUciString(move) << " "; });
        std::cout << '\n' << std::endl;
    }
    std::ignore = begin;
    std::ignore = end;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintMoveInvestigation(const MoveStack::const_iterator end_before_move_generation,
                            const MoveStack::const_iterator move_iterator,
                            const MoveStack::const_iterator end_after_move_generation)
{
    if constexpr (Behavior::debugging)
    {
        const auto number_of_current_move = std::distance(end_before_move_generation, move_iterator);
        const auto total_number_of_pseudo_legal_moves =
            std::distance(end_before_move_generation, end_after_move_generation);
        std::cout << "investigating " << ToUciString(*move_iterator) << " which is #" << number_of_current_move + 1
                  << " of " << total_number_of_pseudo_legal_moves << " pseudo-legal moves\n"
                  << std::endl;
    }
    std::ignore = end_before_move_generation;
    std::ignore = move_iterator;
    std::ignore = end_after_move_generation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintMoveResult(const Bitmove move, const Evaluation minimax_evaluation)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << ToUciString(move) << " = " << minimax_evaluation << '\n' << std::endl;
    }
    std::ignore = move;
    std::ignore = minimax_evaluation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintPruningInfo(const Evaluation negamax_alpha, const Evaluation negamax_beta, const Evaluation negamax_sign)
{
    if constexpr (Behavior::debugging)
    {
        if (negamax_sign > 0)
        {
            std::cout << "alpha = " << negamax_alpha << ", beta = " << negamax_beta << '\n' << std::endl;
        }
        else
        {
            std::cout << "alpha = " << -negamax_beta << ", beta = " << -negamax_alpha << '\n' << std::endl;
        }
    }
    std::ignore = negamax_alpha;
    std::ignore = negamax_beta;
    std::ignore = negamax_sign;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintPruningDecision()
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "pruning!" << '\n' << std::endl;
    }
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

template <typename Behavior>
void PrintPrincipalVariation(const PrincipalVariation& principal_variation,
                             const std::size_t current_depth,
                             const Bitmove current_move)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "principal variation:\n";
        std::cout << "promoted subline after " << ToUciString(current_move) << " at depth " << current_depth << '\n';
        std::cout << ToString(principal_variation) << std::endl;
    }
    std::ignore = current_move;
    std::ignore = current_depth;
    std::ignore = principal_variation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintConsiderationOfPrincipalVariation(const Bitmove move)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "considering from principal variation:\n" << ToString(move) << std::endl;
    }
    std::ignore = move;  // Resolve warning if debugging disabled.
}

/// @brief Overwrites current line with next subline (assuming it was evaluated to be better)
///
/// Copies line starting at current_depth+1 to current_depth in triangular prinicpal variation table.
template <typename DebugBehavior>
inline void PromoteSubline(PrincipalVariation& principal_variation,
                           const std::size_t current_depth,
                           const Bitmove current_move)
{
    const std::size_t current_move_index = GetSublineIndexAtDepth(current_depth);
    const std::size_t next_move_of_subline_index = GetSublineIndexAtDepth(current_depth + 1);
    const std::size_t end_of_subline_index = GetSublineIndexAtDepth(current_depth + 2);
    const auto begin_of_principal_variation = std::begin(principal_variation);
    principal_variation[current_move_index] = current_move;
    std::copy(begin_of_principal_variation + next_move_of_subline_index,
              begin_of_principal_variation + end_of_subline_index,
              begin_of_principal_variation + current_move_index + 1);
}

inline void ClearLine(PrincipalVariation& principal_variation, const std::size_t current_depth)
{
    const std::size_t begin_of_line_index = GetSublineIndexAtDepth(current_depth);
    const std::size_t end_of_line_index = GetSublineIndexAtDepth(current_depth + 1);
    std::fill_n(
        std::begin(principal_variation) + begin_of_line_index, end_of_line_index - begin_of_line_index, kBitNullMove);
}

/// @brief A negamax search using alpha/beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
Evaluation FindBestMove(Position& position,
                        PrincipalVariation& principal_variation,
                        const MoveStack::iterator end_before_move_generation,
                        const Evaluation negamax_sign,
                        const AbortCondition& abort_condition,
                        const std::size_t current_depth = 0,
                        const Evaluation parent_negamax_alpha = std::numeric_limits<Evaluation>::lowest(),
                        const Evaluation parent_negamax_beta = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntry<DebugBehavior>(position, current_depth);
    if (current_depth == abort_condition.full_search_depth)
    {
        const Evaluation minimax_evaluation = Evaluate<EvaluateBehavior>(position);
        PrintEvaluation<DebugBehavior>(minimax_evaluation);
        PrintNodeExit<DebugBehavior>(current_depth);
        return minimax_evaluation * negamax_sign;
    }

    const MoveStack::iterator end_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_before_move_generation);
    std::sort(end_before_move_generation, end_after_move_generation, IsMaterialDifferenceGreater);
    const bool is_inital_entry =
        (current_depth == 0) && (principal_variation[GetSublineIndexAtDepth(1)] == kBitNullMove);
    const bool is_first_entry_into_current_depth{principal_variation[GetSublineIndexAtDepth(current_depth)] ==
                                                 kBitNullMove};
    if (is_inital_entry || is_first_entry_into_current_depth)
    {
        PrintConsiderationOfPrincipalVariation<DebugBehavior>(principal_variation[current_depth]);
        const auto IsMoveSuggestedByPrincipalVariation = [&principal_variation, &current_depth](const auto a,
                                                                                                const auto b) {
            std::ignore = b;
            return a == principal_variation[current_depth];
        };
        std::sort(end_before_move_generation, end_after_move_generation, IsMoveSuggestedByPrincipalVariation);
    }
    PrintGeneratedMoves<DebugBehavior>(end_before_move_generation, end_after_move_generation);

    Evaluation negamax_alpha = parent_negamax_alpha;
    bool is_terminal_node = true;

    for (MoveStack::iterator move_iterator = end_before_move_generation; move_iterator != end_after_move_generation;
         move_iterator++)
    {
        const Bitmove current_move = *move_iterator;
        const Bitboard saved_extras = position.MakeMove(current_move);
        if (!position.IsKingInCheck(position.defending_side_))
        {
            PrintMoveInvestigation<DebugBehavior>(end_before_move_generation, move_iterator, end_after_move_generation);
            is_terminal_node = false;
            Evaluation negamax_evaluation =
                -FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(position,
                                                                                 principal_variation,
                                                                                 end_after_move_generation,
                                                                                 -negamax_sign,
                                                                                 abort_condition,
                                                                                 current_depth + 1,
                                                                                 -parent_negamax_beta,
                                                                                 -negamax_alpha);
            PrintMoveResult<DebugBehavior>(*move_iterator, negamax_evaluation * negamax_sign);

            if (negamax_evaluation > negamax_alpha)
            {
                negamax_alpha = negamax_evaluation;
                PromoteSubline<DebugBehavior>(principal_variation, current_depth, current_move);
                PrintPrincipalVariation<DebugBehavior>(principal_variation, current_depth, current_move);
            }

            PrintPruningInfo<DebugBehavior>(negamax_alpha, parent_negamax_beta, negamax_sign);
        }
        position.UnmakeMove(current_move, saved_extras);

        if (negamax_alpha >= parent_negamax_beta)
        {
            PrintPruningDecision<DebugBehavior>();
            break;
        }
    }

    if (is_terminal_node)
    {
        constexpr Evaluation negamax_draw = Evaluation{0};
        constexpr Evaluation negamax_loss = Evaluation{-1000};
        negamax_alpha = position.IsKingInCheck(position.attacking_side_) ? negamax_loss : negamax_draw;
        PrintEvaluation<DebugBehavior>(negamax_alpha * negamax_sign);
        ClearLine(principal_variation, current_depth);
    }

    PrintNodeExit<DebugBehavior>(current_depth);
    return negamax_alpha;
}

}  // namespace Chess

#endif
