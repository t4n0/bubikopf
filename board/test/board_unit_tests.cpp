#include "board/board.h"

#include "board/piece.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Board, Set_ExpectBoundsCheck) {
  Board board{};
  EXPECT_NO_THROW(board.Get({3, 3}));
  EXPECT_THROW(board.Get({-1, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({8, 3}), std::out_of_range);
  EXPECT_THROW(board.Get({3, -1}), std::out_of_range);
  EXPECT_THROW(board.Get({3, 8}), std::out_of_range);

  constexpr Player player{Player::max};
  EXPECT_NO_THROW(board.Set({3, 3}, Knight::OfSide(player)));
  EXPECT_THROW(board.Set({-1, 3}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({8, 3}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, -1}, Knight::OfSide(player)), std::out_of_range);
  EXPECT_THROW(board.Set({3, 8}, Knight::OfSide(player)), std::out_of_range);
}

TEST(Board, StreamOutputOperator_MustNotThrow) {
  const State state{};
  EXPECT_NO_THROW(std::cout << state.board_ << '\n');
}

TEST(ToIdxAndToCoor, GivenAConversionThereAndBack_ExpectUnalteredValue) {
  const Coordinate coordinate{4, 3};
  const std::size_t index{28};

  EXPECT_TRUE(index == ToIdx(coordinate));
  EXPECT_TRUE(coordinate == ToCoor(index));
}

TEST(ToIdx, GivenSquareNotation_ExpectIndex) {
  const std::size_t a8_idx = 0;
  const std::size_t h8_idx = 7;
  const std::size_t a1_idx = 56;
  const std::size_t h1_idx = 63;
  EXPECT_EQ(ToIdx('a', 1), a1_idx);
  EXPECT_EQ(ToIdx('a', 8), a8_idx);
  EXPECT_EQ(ToIdx('h', 1), h1_idx);
  EXPECT_EQ(ToIdx('h', 8), h8_idx);
}

TEST(ToIdx, GivenUciNotation_ExpectIndex) {
  const std::size_t a8_idx = 0;
  const std::size_t h8_idx = 7;
  const std::size_t a1_idx = 56;
  const std::size_t h1_idx = 63;
  EXPECT_EQ(ToIdx("a1"), a1_idx);
  EXPECT_EQ(ToIdx("a8"), a8_idx);
  EXPECT_EQ(ToIdx("h1"), h1_idx);
  EXPECT_EQ(ToIdx("h8"), h8_idx);
}

TEST(ToIdx, GivenIndex_ExpectUciNotation) {
  EXPECT_EQ(ToUciSquare(0), "a8");
  EXPECT_EQ(ToUciSquare(7), "h8");
  EXPECT_EQ(ToUciSquare(56), "a1");
  EXPECT_EQ(ToUciSquare(63), "h1");
}

}  // namespace
}  // namespace Chess
