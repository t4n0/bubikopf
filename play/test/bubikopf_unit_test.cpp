#include "bitboard/uci_conversion.h"
#include "play/bubikopf.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess
{
namespace
{

class BubikopfTestFixture : public testing::Test
{
  public:
    Bubikopf engine_api{};
};

TEST_F(BubikopfTestFixture, GivenGuiMoveListToShort_ExpectThrowsRuntimeError)
{
    engine_api.UpdateBoard({"e2e4"});  // Play one move
    EXPECT_THROW(engine_api.UpdateBoard({}),
                 std::runtime_error);  // To few moves provided
}

TEST_F(BubikopfTestFixture, GivenGuiMoveNotPlayableFromCurrentPosition_ExpectThrowsRuntimeError)
{
    const auto impossible_move = "a1h7";  // Is equivalent to legal move that can't be played from current position
    EXPECT_THROW(engine_api.UpdateBoard({impossible_move}), std::runtime_error);
}

TEST_F(BubikopfTestFixture, DISABLED_GivenSelfPlay_ExpectGameFinishes)  // test takes to long during regular development
{
    std::vector<std::string> move_list{};
    bool game_over = false;
    Evaluation game_result;
    do
    {
        engine_api.UpdateBoard(move_list);
        std::string move_played;
        std::tie(move_played, game_result) = engine_api.FindBestMove();
        move_list.push_back(move_played);
        game_over = move_list.back() == kUciNullMove;

        engine_api.PrintBoard();
        std::cout << move_played << std::endl << std::endl;

    } while (!game_over);

    std::cout << "Game result = " << game_result << '\n' << std::endl;
}

class BubikopfFindBestMoveTestFixture
    : public BubikopfTestFixture,
      public testing::WithParamInterface<std::tuple<std::string, std::string, Evaluation>>
{
  public:
    std::string GetFen() { return std::get<0>(GetParam()); }
    std::string GetExpectedBestMove() { return std::get<1>(GetParam()); }
    Evaluation GetExpectedEvaluation() { return std::get<2>(GetParam()); }
};

TEST_P(BubikopfFindBestMoveTestFixture, GivenCheckmateIn4_ExpectCorrectContinuation)
{
    // Setup
    engine_api.SetUpBoardAccordingToFen(GetFen());

    // Call
    const auto [best_move, evaluation] = engine_api.FindBestMove();

    // Expect
    EXPECT_EQ(best_move, GetExpectedBestMove());
    EXPECT_EQ(evaluation, GetExpectedEvaluation());
}

const std::array<std::tuple<std::string, std::string, Evaluation>, 2> kCheckMateInThreePositions{{
    {"r4k2/pp2qp2/8/3N3r/3P4/1Q4p1/PP4P1/R4RK1 b - - 0 22", "h5h1", Evaluation{-1000}},
    {"r4k2/p3R3/2p1R1pp/1p3pN1/6n1/8/PPPr2PP/7K w - - 14 32", "e7f7", Evaluation{1000}},
}};

INSTANTIATE_TEST_SUITE_P(CheckMateInThreePositions,
                         BubikopfFindBestMoveTestFixture,
                         testing::ValuesIn(kCheckMateInThreePositions));

}  // namespace
}  // namespace Chess