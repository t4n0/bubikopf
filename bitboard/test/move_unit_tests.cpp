#include "bitboard/move.h"

#include <gtest/gtest.h>

#include <limits>

namespace Chess {
namespace {

constexpr std::array<move_t, 4> ALL_MOVE_MASKS{
    MOVE_MASK_SOURCE,
    MOVE_MASK_TARGET,
    MOVE_MASK_STATIC,
    MOVE_MASK_EXTRAS,
};

TEST(MoveMaskTest,
     GivenAllMoveMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized) {
  move_t covered_fields = 0;
  for (const move_t mask : ALL_MOVE_MASKS) {
    covered_fields |= mask;
  }
  EXPECT_EQ(covered_fields, std::numeric_limits<move_t>::max());
}

class MoveMaskFixture
    : public ::testing::TestWithParam<std::tuple<move_t, move_t>> {};

TEST_P(MoveMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap) {
  const move_t first = std::get<0>(GetParam());
  const move_t second = std::get<1>(GetParam());
  if (first != second) {  // only check permutations with different masks
    EXPECT_FALSE(first & second);
  }
}

INSTANTIATE_TEST_SUITE_P(
    AllCombinations, MoveMaskFixture,
    ::testing::Combine(::testing::ValuesIn(ALL_MOVE_MASKS),
                       ::testing::ValuesIn(ALL_MOVE_MASKS)));

constexpr std::array<move_t, 8> ALL_MOVE_MASK_EXTRAS_SUBMASKS{
    MOVE_MASK_EXTRAS_IS_INVERTIBLE,
    MOVE_MASK_EXTRAS_CAPTURED_PIECE,
    MOVE_MASK_EXTRAS_MOVED_PIECE,
    MOVE_MASK_EXTRAS_PROMOTION,
    MOVE_MASK_EXTRAS_IS_EN_PASSENT,
    MOVE_MASK_EXTRAS_IS_KINGSIDE_CASLTLING,
    MOVE_MASK_EXTRAS_IS_QUEENSIDE_CASLTLING,
    MOVE_MASK_EXTRAS_UNUSED,
};

TEST(MoveMaskExtrasTest, GivenAllSubmasks_ExpectEntireRangeOfMaskIsUtilized) {
  move_t covered_fields = 0;
  for (const move_t submask : ALL_MOVE_MASK_EXTRAS_SUBMASKS) {
    covered_fields |= submask;
  }
  EXPECT_EQ(covered_fields, MOVE_MASK_EXTRAS);
}

class MoveMaskExtrasFixture
    : public ::testing::TestWithParam<std::tuple<move_t, move_t>> {};

TEST_P(MoveMaskExtrasFixture, GivenTwoDifferentSubmasks_ExpectNoOverlap) {
  const move_t first = std::get<0>(GetParam());
  const move_t second = std::get<1>(GetParam());
  if (first != second) {  // only check permutations with different masks
    EXPECT_FALSE(first & second);
  }
}

INSTANTIATE_TEST_SUITE_P(
    AllCombinations, MoveMaskExtrasFixture,
    ::testing::Combine(::testing::ValuesIn(ALL_MOVE_MASK_EXTRAS_SUBMASKS),
                       ::testing::ValuesIn(ALL_MOVE_MASK_EXTRAS_SUBMASKS)));

TEST(MoveShiftTargetTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ((MOVE_ONES & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET, 0b00111111);
}

TEST(MoveShiftStaticTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ((MOVE_ONES & MOVE_MASK_STATIC) >> MOVE_SHIFT_STATIC, 0b00111111);
}

TEST(MoveShiftExtrasCapturedPieceTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ((MOVE_ONES & MOVE_MASK_EXTRAS_CAPTURED_PIECE) >>
                MOVE_SHIFT_EXTRAS_CAPTURED_PIECE,
            0b00000111);
}

TEST(MoveShiftExtrasMovedPieceTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ((MOVE_ONES & MOVE_MASK_EXTRAS_MOVED_PIECE) >>
                MOVE_SHIFT_EXTRAS_MOVED_PIECE,
            0b00000111);
}

TEST(MoveShiftExtrasPromotionTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ(
      (MOVE_ONES & MOVE_MASK_EXTRAS_PROMOTION) >> MOVE_SHIFT_EXTRAS_PROMOTION,
      0b00000111);
}

}  // namespace
}  // namespace Chess
