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
    BubikopfTestFixture() { engine_api.RestartGame(); }

    Bubikopf engine_api{};
};

TEST_F(BubikopfTestFixture, GivenGuiMoveListToShort_ExpectThrowsRuntimeError)
{
    engine_api.UpdateBoard({"e2e4"});                                                 // Play one move
    EXPECT_DEATH(engine_api.UpdateBoard({}), "Move list from gui is behind engine");  // To few moves provided
}

TEST_F(BubikopfTestFixture, GivenGuiMoveNotPlayableFromCurrentPosition_ExpectThrowsRuntimeError)
{
    const auto impossible_move = "a1h7";  // Is equivalent to legal move that can't be played from current position
    EXPECT_DEATH(engine_api.UpdateBoard({impossible_move}),
                 "Move played by gui not possible from internal representation");
}

TEST_F(BubikopfTestFixture, DISABLED_GivenSelfPlay_ExpectGameFinishes)
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

}  // namespace
}  // namespace Chess