#include "board/player.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(PlayerTest, NegationOperator)
{
    // Setup
    const Player maximizing_player = Player::max;
    const Player minimizing_player = Player::min;

    // Call
    const Player negation_of_maximizing_player = !maximizing_player;
    const Player negation_of_minimizing_player = !minimizing_player;

    // Expect
    EXPECT_EQ(negation_of_maximizing_player, Player::min);
    EXPECT_EQ(negation_of_minimizing_player, Player::max);
}

}  // namespace
}  // namespace Chess
