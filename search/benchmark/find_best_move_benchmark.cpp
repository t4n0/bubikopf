#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "evaluate/evaluate.h"
#include "search/find_best_move.h"

#include <benchmark/benchmark.h>

namespace
{

const char* const kStartPositionFen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
const char* const kMiddleGameFen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
const char* const kEndGameFen = "8/2p5/1P1p4/2P3rk/KR3p2/4P1p1/5P2/8 w - - 0 1";
constexpr Chess::Evaluation kNegamaxEvaluationSignWhite{1};

}  // namespace

static void FindBestMove(benchmark::State& state)
{
    Chess::MoveStack move_stack{};
    Chess::PrincipalVariation principal_variation{};
    Chess::Position start_position = Chess::PositionFromFen(kStartPositionFen);
    Chess::Position middle_game = Chess::PositionFromFen(kMiddleGameFen);
    Chess::Position end_game = Chess::PositionFromFen(kEndGameFen);
    constexpr std::size_t full_search_depth = 6;
    constexpr Chess::AbortCondition abort_condition{full_search_depth};

    for (auto _ : state)
    {
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            start_position, principal_variation, move_stack.begin(), kNegamaxEvaluationSignWhite, abort_condition);
        principal_variation.fill(Chess::kBitNullMove);
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            middle_game, principal_variation, move_stack.begin(), kNegamaxEvaluationSignWhite, abort_condition);
        principal_variation.fill(Chess::kBitNullMove);
        Chess::FindBestMove<Chess::GenerateAllPseudoLegalMoves, Chess::EvaluateMaterial>(
            end_game, principal_variation, move_stack.begin(), kNegamaxEvaluationSignWhite, abort_condition);
    }
}
BENCHMARK(FindBestMove)->Unit(benchmark::kMillisecond)->ReportAggregatesOnly()->Repetitions(10);

BENCHMARK_MAIN();