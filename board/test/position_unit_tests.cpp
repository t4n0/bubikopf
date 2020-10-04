#include "board/position.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(SetGetCastling, ExpectRespectiveCastlingRights)
{
    // Setup
    Position position{};
    const Castling expected_castling_white{true, false};
    const Castling expected_castling_black{false, true};

    // Call
    position.SetCastling(Player::max, expected_castling_white);
    position.SetCastling(Player::min, expected_castling_black);
    const Castling returned_castling_white = position.GetCastling(Player::max);
    const Castling returned_castling_black = position.GetCastling(Player::min);

    // Expect
    EXPECT_TRUE(expected_castling_white.queenside == returned_castling_white.queenside);
    EXPECT_TRUE(expected_castling_white.kingside == returned_castling_white.kingside);
    EXPECT_TRUE(expected_castling_black.queenside == returned_castling_black.queenside);
    EXPECT_TRUE(expected_castling_black.kingside == returned_castling_black.kingside);
}

TEST(Position, GivenDefaultCtor_ExpectBoardIsFilledWithEmptySquares)
{
    const Position position{};
    for (const auto& square : position.board_.squares_)
    {
        EXPECT_TRUE(square->IsEmpty());
    }
}

TEST(Position_GetTurn, GivenEvenPly_ExpectWhitesTurn)
{
    Position position{};
    position.plies_ = 0;
    EXPECT_EQ(position.GetTurn(), Player::max);
    position.plies_ = 2;
    EXPECT_EQ(position.GetTurn(), Player::max);
    position.plies_ = 4;
    EXPECT_EQ(position.GetTurn(), Player::max);
}

TEST(Position_GetTurn, GivenUnwvenPly_ExpectBlacksTurn)
{
    Position position{};
    position.plies_ = 1;
    EXPECT_EQ(position.GetTurn(), Player::min);
    position.plies_ = 3;
    EXPECT_EQ(position.GetTurn(), Player::min);
    position.plies_ = 5;
    EXPECT_EQ(position.GetTurn(), Player::min);
}

}  // namespace
}  // namespace Chess
