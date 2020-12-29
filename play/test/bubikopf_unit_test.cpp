#include "play/bubikopf.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

class BubikopfUpdateBoardTestFixture : public testing::Test
{
  public:
    BubikopfUpdateBoardTestFixture() { engine_api.RestartGame(); }

    Bubikopf engine_api{};
};

TEST_F(BubikopfUpdateBoardTestFixture, GivenGuiMoveListToShort_ExpectThrowsRuntimeError)
{
    engine_api.UpdateBoard({"e2e4"});                              // Play one move
    EXPECT_THROW(engine_api.UpdateBoard({}), std::runtime_error);  // To few moves provided
}

TEST_F(BubikopfUpdateBoardTestFixture, GivenGuiMoveNotPlayableFromCurrentPosition_ExpectThrowsRuntimeError)
{
    const auto impossible_move = "a1h7";  // Is equivalent to legal move that can't be played from current position
    EXPECT_THROW(engine_api.UpdateBoard({impossible_move}), std::runtime_error);
}

}  // namespace
}  // namespace Chess