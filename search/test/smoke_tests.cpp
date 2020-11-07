#include "search/test/evaluate_mock.h"
#include "search/test/minimax_mock.h"
#include "search/test/move_generation_mock.h"

#include <gtest/gtest.h>

#include <iostream>
#include <chrono>

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
    PositionWithBitboards position = SetUpStandardStartPosition();
    MoveList move_list{};

    // Call
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    minimax<SearchAllBranchesWithoutPruning, GenerateAllPseudoLegalMoves, EvaluteToZeroAndCount>(
        GetParam().depth, position, move_list.begin());
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // Expect
    EXPECT_EQ(GetParam().expected_number_of_nodes, EvaluteToZeroAndCount::number_of_evaluations) << ToString(move_list);
    std::cout << "Nodes evaluated = " << EvaluteToZeroAndCount::number_of_evaluations << std::endl;
    std::cout << "Time spent = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]"
              << std::endl;
}

// Numbers taken from https://www.chessprogramming.org/Perft_Results
constexpr MiniMaxTestParameter ply1{1, 20};
constexpr MiniMaxTestParameter ply2{2, 400};
constexpr MiniMaxTestParameter ply3{3, 8902};
constexpr MiniMaxTestParameter ply4{4, 197281};  // castling missing, also checks
constexpr MiniMaxTestParameter ply5{5, 4865609};
constexpr MiniMaxTestParameter ply6{6, 119060324};
constexpr MiniMaxTestParameter ply7{7, 3195901860};
constexpr MiniMaxTestParameter ply8{8, 84998978956};
constexpr MiniMaxTestParameter ply9{9, 2439530234167};

INSTANTIATE_TEST_SUITE_P(FromStartPosition, MiniMaxTestFixture, ::testing::Values(ply3));

}  // namespace
}  // namespace Chess
