#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "evaluate/evaluate.h"
#include "search/find_best_move.h"
#include "search/traverse_all_leaves.h"

#include <benchmark/benchmark.h>

const char* const starting_position_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
const char* const middle_game_fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
const char* const end_game_fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";

static void TraverseAllLeaves(benchmark::State& state)
{
    Chess::MoveStack move_stack{};
    Chess::Statistic stats{};
    constexpr int depth{4};

    Chess::Position starting_position = Chess::PositionFromFen(starting_position_fen);
    Chess::Position middle_game = Chess::PositionFromFen(starting_position_fen);
    Chess::Position end_game = Chess::PositionFromFen(starting_position_fen);

    for (auto _ : state)
    {
        Chess::TraverseAllLeaves<Chess::GenerateAllPseudoLegalMoves>(
            starting_position, depth, move_stack.begin(), stats);
        Chess::TraverseAllLeaves<Chess::GenerateAllPseudoLegalMoves>(middle_game, depth, move_stack.begin(), stats);
        Chess::TraverseAllLeaves<Chess::GenerateAllPseudoLegalMoves>(end_game, depth, move_stack.begin(), stats);
    }
}

BENCHMARK(TraverseAllLeaves)->Unit(benchmark::kMillisecond)->ReportAggregatesOnly()->Repetitions(10);

static void FindBestMove(benchmark::State& state)
{
    Chess::MoveStack move_stack{};
    constexpr int depth{6};
    constexpr Chess::Evaluation negamax_sign_white{1};

    Chess::Position starting_position = Chess::PositionFromFen(starting_position_fen);
    Chess::Position middle_game = Chess::PositionFromFen(starting_position_fen);
    Chess::Position end_game = Chess::PositionFromFen(starting_position_fen);

    for (auto _ : state)
    {
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            starting_position, depth, move_stack.begin(), negamax_sign_white);
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            middle_game, depth, move_stack.begin(), negamax_sign_white);
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            end_game, depth, move_stack.begin(), negamax_sign_white);
    }
}

BENCHMARK(FindBestMove)->Unit(benchmark::kMillisecond)->ReportAggregatesOnly()->Repetitions(10);

BENCHMARK_MAIN();