#include "bitboard/position_from_fen.h"
#include "evaluate/test/evaluate_mock.h"
#include "search/test/minimax_mock.h"
#include "search/test/move_generation_mock.h"

#include <benchmark/benchmark.h>

const std::array<const char* const, 5> fens{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                                            "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                                            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                                            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                                            "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"};

int Chess::EvaluteToZeroAndCount::number_of_evaluations = 0;

static void FullSearch(benchmark::State& state)
{
    Chess::PositionWithBitboards position = Chess::PositionFromFen(fens.at(state.range(0)));
    Chess::MoveList move_list{};

    for (auto _ : state)
    {
        Chess::minimax<Chess::SearchAllBranchesWithoutPruning,
                       Chess::GenerateAllPseudoLegalMoves,
                       Chess::EvaluteToZeroAndCount>(3, position, move_list.begin());
    }
}

BENCHMARK(FullSearch)
    ->Arg(0)
    ->Arg(1)
    ->Arg(2)
    ->Arg(3)
    ->Arg(4)
    ->Unit(benchmark::kMillisecond)
    ->ReportAggregatesOnly()
    ->Repetitions(5);

BENCHMARK_MAIN();