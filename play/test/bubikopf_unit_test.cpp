#include "play/bubikopf.h"

#include <gtest/gtest.h>

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

}  // namespace
}  // namespace Chess