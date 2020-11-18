#include "bitboard/position_from_fen.h"
#include "evaluate/test/evaluate_mock.h"
#include "search/test/minimax_mock.h"
#include "search/test/move_generation_mock.h"

#include <gtest/gtest.h>

#include <chrono>
#include <iostream>

namespace Chess
{

int EvaluteToZeroAndCount::number_of_evaluations{0};

namespace
{

TEST(MoveListTest, GivenDepth3_ExpectDebuggingIdsOf6LastVisitedMovesInMoveList)
{
    // Setup
    PositionWithBitboards position{};
    MoveList move_list{};
    const int DEPTH{3};

    // Call
    minimax<SearchAllBranchesWithoutPruning, GenerateTwoMovesWithUniqueDebugId, EvaluteToZeroAndCount>(
        DEPTH, position, move_list.begin());

    // Expect
    const std::array<Bitmove, 6> expected_debugging_ids{1, 2, 9, 10, 13, 14};  // worked out by hand
    for (std::size_t idx{0}; idx < expected_debugging_ids.size(); idx++)
    {
        EXPECT_EQ(expected_debugging_ids.at(idx), move_list.at(idx));
    }
}

struct MiniMaxTestParameter
{
    int depth;
    std::string fen;
    long long expected_number_of_nodes;
};

class MiniMaxTestFixture : public ::testing::TestWithParam<MiniMaxTestParameter>
{
  public:
    void SetUp() final { EvaluteToZeroAndCount::number_of_evaluations = 0; }
    void TearDown() final {}
};

TEST_P(MiniMaxTestFixture, GivenDepth_ExpectCorrectNumberOfEvaluations)
{
    // Setup
    PositionWithBitboards position = PositionFromFen(GetParam().fen);
    MoveList move_list{};

    // Call
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    minimax<SearchAllBranchesWithoutPruning, GenerateAllPseudoLegalMoves, EvaluteToZeroAndCount>(
        GetParam().depth, position, move_list.begin());
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Expect
    EXPECT_EQ(GetParam().expected_number_of_nodes, EvaluteToZeroAndCount::number_of_evaluations) << ToString(move_list);
    std::cout << "Time spent = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]"
              << std::endl;
}

// Numbers taken from https://www.chessprogramming.org/Perft_Results
const char* const pos2_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
const char* const pos3_fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
const char* const pos4_fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
const char* const pos5_fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
const char* const pos6_fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

const MiniMaxTestParameter pos1_ply4{4, kStandardStartingPosition, 197281};
const MiniMaxTestParameter pos2_ply4{4, pos2_fen, 4085603};
const MiniMaxTestParameter pos3_ply4{4, pos3_fen, 43238};
const MiniMaxTestParameter pos4_ply4{4, pos4_fen, 422333};
const MiniMaxTestParameter pos5_ply4{4, pos5_fen, 2103487};
const MiniMaxTestParameter pos6_ply4{4, pos6_fen, 3894594};

INSTANTIATE_TEST_SUITE_P(FromPositionDepth4,
                         MiniMaxTestFixture,
                         ::testing::Values(pos1_ply4, pos2_ply4, pos3_ply4, pos4_ply4, pos5_ply4, pos6_ply4));

const MiniMaxTestParameter pos1_ply6{6, kStandardStartingPosition, 119060324};
const MiniMaxTestParameter pos2_ply6{6, pos2_fen, 8031647685};
const MiniMaxTestParameter pos3_ply6{6, pos3_fen, 11030083};
const MiniMaxTestParameter pos4_ply6{6, pos4_fen, 706045033};
const MiniMaxTestParameter pos5_ply5{5, pos5_fen, 89941194};  // numbers for depth 6 not available
const MiniMaxTestParameter pos6_ply6{6, pos6_fen, 6923051137};

// Disabled by default. Takes <2h.
// INSTANTIATE_TEST_SUITE_P(FromPositionDepth6,
//                          MiniMaxTestFixture,
//                          ::testing::Values(pos1_ply6, pos2_ply6, pos3_ply6, pos4_ply6, pos5_ply5, pos6_ply6));

}  // namespace
}  // namespace Chess
