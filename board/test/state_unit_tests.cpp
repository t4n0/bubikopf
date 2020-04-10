#include "board/state.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(SetGetCastling, ExpectRespectiveCastlingRights) {
  // Setup
  State state{};
  const Castling expected_castling_white{true, false};
  const Castling expected_castling_black{false, true};

  // Call
  state.SetCastling(Player::max, expected_castling_white);
  state.SetCastling(Player::min, expected_castling_black);
  const Castling returned_castling_white = state.GetCastling(Player::max);
  const Castling returned_castling_black = state.GetCastling(Player::min);

  // Expect
  EXPECT_TRUE(expected_castling_white.queenside ==
              returned_castling_white.queenside);
  EXPECT_TRUE(expected_castling_white.kingside ==
              returned_castling_white.kingside);
  EXPECT_TRUE(expected_castling_black.queenside ==
              returned_castling_black.queenside);
  EXPECT_TRUE(expected_castling_black.kingside ==
              returned_castling_black.kingside);
}

TEST(State, MemoryFootprint) {
  EXPECT_EQ(sizeof(State), 536);
  // consisting of
  EXPECT_EQ(sizeof(Board), 512);
  EXPECT_EQ(sizeof(int), 4);  // x2 = 8
  EXPECT_EQ(sizeof(Player), 4);
  EXPECT_EQ(sizeof(std::optional<Coordinate>), 3);
  EXPECT_EQ(sizeof(Castling), 2);  // x2 = 4
  // + padding
}

TEST(State, GivenDefaultCtor_ExpectBoardIsFilledWithEmptySquares) {
  const State state{};
  for (const auto& square : state.board_.squares_) {
    EXPECT_TRUE(square->IsEmpty());
  }
}

}  // namespace
}  // namespace Chess
