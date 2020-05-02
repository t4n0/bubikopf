#include "bitboard/position.h"

#include <gtest/gtest.h>

#include <limits>

namespace Chess {
namespace {

constexpr std::array<bitboard_t, 5> ALL_BOARD_MASKS{
    BOARD_MASK_STATIC_PLIES, BOARD_MASK_EN_PASSENT, BOARD_MASK_CASTLING,
    BOARD_MASK_WHITE_TURN,   BOARD_MASK_UNUSED,
};

TEST(BoardMaskTest,
     GivenAllBoardMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized) {
  bitboard_t covered_fields = 0;
  for (const bitboard_t mask : ALL_BOARD_MASKS) {
    covered_fields |= mask;
  }
  EXPECT_EQ(covered_fields, std::numeric_limits<bitboard_t>::max());
}

class BoardMaskFixture
    : public ::testing::TestWithParam<std::tuple<bitboard_t, bitboard_t>> {};

TEST_P(BoardMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap) {
  const bitboard_t first = std::get<0>(GetParam());
  const bitboard_t second = std::get<1>(GetParam());
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

TEST(BoardIndxesTest,
     GivenUniqueNumberToEachBitboard_ExpectRespectiveUniqueNumber) {
  PositionWithBitboards unit{};
  unit.boards[BOARD_IDX_EXTRAS] = 1;
  unit.boards[BOARD_IDX_WHITE] = 2;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_PAWNS] = 3;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_KNIGHTS] = 4;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_BISHOPS] = 5;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_ROOKS] = 6;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_QUEENS] = 7;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_KINGS] = 8;
  unit.boards[BOARD_IDX_WHITE + BOARD_IDX_ATTACK] = 9;
  unit.boards[BOARD_IDX_BLACK] = 10;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_PAWNS] = 11;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_KNIGHTS] = 12;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_BISHOPS] = 13;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_ROOKS] = 14;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_QUEENS] = 15;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_KINGS] = 16;
  unit.boards[BOARD_IDX_BLACK + BOARD_IDX_ATTACK] = 17;

  bitboard_t expected_unique_number = 1;
  for (bitboard_t board : unit.boards) {
    EXPECT_EQ(board, expected_unique_number);
    expected_unique_number++;
  }
}

}  // namespace
}  // namespace Chess
