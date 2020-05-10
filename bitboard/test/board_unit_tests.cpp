#include "bitboard/board.h"

#include <gtest/gtest.h>

#include <limits>

namespace Chess {
namespace {

constexpr std::array<Bitboard, 5> ALL_BOARD_MASKS{
    BOARD_MASK_STATIC_PLIES, BOARD_MASK_EN_PASSENT, BOARD_MASK_CASTLING,
    BOARD_MASK_WHITE_TURN,   BOARD_MASK_UNUSED,
};

TEST(BoardMaskTest,
     GivenAllBoardMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized) {
  Bitboard covered_fields = 0;
  for (const Bitboard mask : ALL_BOARD_MASKS) {
    covered_fields |= mask;
  }
  EXPECT_EQ(covered_fields, std::numeric_limits<Bitboard>::max());
}

class BoardMaskFixture
    : public ::testing::TestWithParam<std::tuple<Bitboard, Bitboard>> {};

TEST_P(BoardMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap) {
  const Bitboard first = std::get<0>(GetParam());
  const Bitboard second = std::get<1>(GetParam());
  if (first != second) {  // only check permutations with different masks
    EXPECT_FALSE(first & second);
  }
}

INSTANTIATE_TEST_SUITE_P(
    AllCombinations, BoardMaskFixture,
    ::testing::Combine(::testing::ValuesIn(ALL_BOARD_MASKS),
                       ::testing::ValuesIn(ALL_BOARD_MASKS)));

TEST(BoardShiftEnPassentTest, GivenOnAllOnes_Expect64) {
  EXPECT_EQ((BOARD_ONES & BOARD_MASK_EN_PASSENT) >> BOARD_SHIFT_EN_PASSENT,
            0b00111111);
}

TEST(BoardShiftCastlingTest, GivenOnAllOnes_Expect4) {
  EXPECT_EQ((BOARD_ONES & BOARD_MASK_CASTLING) >> BOARD_SHIFT_CASTLING,
            0b00001111);
}

}  // namespace
}  // namespace Chess
