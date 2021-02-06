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
void PrintEvaluation(const Evaluation evaluation)
{
    if constexpr (Behavior::debugging)
    {
        std::cout << "= " << evaluation << '\n' << std::endl;
    }
    std::ignore = evaluation;  // Resolve warning if debugging disabled.
}

template <typename Behavior>
void PrintMove(const MoveStack::const_iterator end_before_move_generation,
               const MoveStack::const_iterator move_iterator,
               const MoveStack::const_iterator end_after_move_generation)
{
    if constexpr (Behavior::debugging)
    {
        const auto number_of_current_move = std::distance(end_before_move_generation, move_iterator);
        const auto total_number_of_pseudo_legal_moves =
            std::distance(end_before_move_generation, end_after_move_generation);
        std::cout << "investigating " << ToUciString(*move_iterator) << " which is #" << number_of_current_move
                  << " of " << total_number_of_pseudo_legal_moves << " pseudo-legal moves\n"
                  << std::endl;
    }
    std::ignore = end_before_move_generation;
    std::ignore = move_iterator;
    std::ignore = end_after_move_generation;  // Resolve warning if debugging disabled.
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

template <typename Behavior>
void PrintPrincipalVariation(const PrincipalVariation& principal_variation,
                             const std::size_t current_depth,
                             const Bitmove current_move)
{
    if constexpr (Behavior::debugging)
    {
        constexpr const char* const separator = " ";                                 // one space
        constexpr const char* const empty_promotion = " ";                           // one space
        constexpr const char* const uci_move_placeholder_with_max_length = "     ";  // five spaces

        int line{0};  // 0 is the main line, 1 the subline after first move of main line, etc.
        std::cout << "Principal variation:\n";
        std::cout << "Promoted subline after " << ToUciString(current_move) << " at depth " << current_depth << '\n';
        for (std::size_t index{0}; index < principal_variation.size(); index++)
        {
            const bool end_of_line_reached = (index == GetSublineIndexAtDepth(line + 1));
            if (end_of_line_reached)
            {
                const bool is_main_line{line == 0};
                if (is_main_line)
                {
                    std::cout << "(main line)";
                }
                line++;
                std::cout << '\n';
                for (int count{0}; count < line; count++)
                {
                    std::cout << uci_move_placeholder_with_max_length << separator;
                }
            }
            const std::string uci_move{ToUciString(principal_variation[index])};
            std::cout << uci_move << separator;
            const bool is_move_without_promotion{uci_move.size() == 4};
            if (is_move_without_promotion)
            {
                std::cout << empty_promotion;  // Make sure printout is aligned for promotions, too.
            }
        }
        std::cout << '\n' << std::endl;
    }
    std::ignore = current_move;
    std::ignore = current_depth;
    std::ignore = principal_variation;  // Resolve warning if debugging disabled.
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

/// @brief A negamax search using alpha/beta pruning.
template <typename GenerateBehavior, typename EvaluateBehavior, typename DebugBehavior = DebuggingDisabled>
Evaluation FindBestMove(Position& position,
                        PrincipalVariation& principal_variation,
                        const MoveStack::iterator end_before_move_generation,
                        const Evaluation negamax_sign,
                        const AbortCondition& abort_condition,
                        const std::size_t current_depth = 0,
                        const Evaluation alpha_parent = std::numeric_limits<Evaluation>::lowest(),
                        const Evaluation beta_parent = std::numeric_limits<Evaluation>::max())
{
    PrintNodeEntry<DebugBehavior>(position, current_depth);
    const bool final_depth_of_full_search_reached{current_depth == abort_condition.full_search_depth};
    if (final_depth_of_full_search_reached)
    {
        const Evaluation evaluation = Evaluate<EvaluateBehavior>(position);
        PrintEvaluation<DebugBehavior>(evaluation);
        PrintNodeExit<DebugBehavior>(current_depth);
        return evaluation * negamax_sign;
    }

    const MoveStack::iterator end_after_move_generation =
        GenerateMoves<GenerateBehavior>(position, end_before_move_generation);
    std::sort(end_before_move_generation, end_after_move_generation, IsMaterialDifferenceGreater);

    Evaluation alpha = alpha_parent;
    bool is_terminal_node = true;

    for (MoveStack::iterator move_iterator = end_before_move_generation; move_iterator != end_after_move_generation;
         move_iterator++)
    {
        const Bitmove current_move = *move_iterator;
        const Bitboard saved_extras = position.MakeMove(current_move);
        const bool move_is_legal = !position.IsKingInCheck(position.defending_side_);
        if (move_is_legal)
        {
            PrintMove<DebugBehavior>(end_before_move_generation, move_iterator, end_after_move_generation);
            is_terminal_node = false;
            Evaluation eval =
                -FindBestMove<GenerateBehavior, EvaluateBehavior, DebugBehavior>(position,
                                                                                 principal_variation,
                                                                                 end_after_move_generation,
                                                                                 -negamax_sign,
                                                                                 abort_condition,
                                                                                 current_depth + 1,
                                                                                 -beta_parent,
                                                                                 -alpha);

            const bool current_move_is_best_so_far{eval > alpha};
            if (current_move_is_best_so_far)
            {
                alpha = eval;
                PromoteSubline<DebugBehavior>(principal_variation, current_depth, current_move);
                PrintPrincipalVariation<DebugBehavior>(principal_variation, current_depth, current_move);
            }
        }
        position.UnmakeMove(current_move, saved_extras);

        const bool opponent_has_better_option{alpha >= beta_parent};
        if (opponent_has_better_option)
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
        return game_result * negamax_sign;
    }

    PrintNodeExit<DebugBehavior>(current_depth);
    return alpha;
}

}  // namespace Chess

#endif
