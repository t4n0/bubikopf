#include "bitboard/move.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>

namespace Chess
{
namespace
{

constexpr std::array<Bitmove, 7> ALL_MOVE_MASKS{
    MOVE_MASK_SOURCE,
    MOVE_MASK_TARGET,
    MOVE_MASK_MOVED_PIECE,
    MOVE_MASK_CAPTURED_PIECE,
    MOVE_MASK_PROMOTION,
    MOVE_MASK_TYPE,
    MOVE_MASK_UNUSED,
};

// Mask tests
TEST(MoveMaskTest, GivenAllMoveMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized)
{
    Bitmove covered_fields = 0;
    for (const Bitmove mask : ALL_MOVE_MASKS)
    {
        covered_fields |= mask;
    }
    EXPECT_EQ(covered_fields, std::numeric_limits<Bitmove>::max());
}

class MoveMaskFixture : public ::testing::TestWithParam<std::tuple<Bitmove, Bitmove>>
{
};

TEST_P(MoveMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap)
{
    const Bitmove first = std::get<0>(GetParam());
    const Bitmove second = std::get<1>(GetParam());
    if (first != second)
    {  // only check permutations with different masks
        EXPECT_FALSE(first & second);
    }
}

INSTANTIATE_TEST_SUITE_P(AllCombinations,
                         MoveMaskFixture,
                         ::testing::Combine(::testing::ValuesIn(ALL_MOVE_MASKS), ::testing::ValuesIn(ALL_MOVE_MASKS)));

// Value tests
const std::array<Bitmove, 8> ALL_MOVE_VALUES{
    MOVE_VALUE_TYPE_QUIET_NON_PAWN,
    MOVE_VALUE_TYPE_CAPTURE,
    MOVE_VALUE_TYPE_PAWN_PUSH,
    MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH,
    MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE,
    MOVE_VALUE_TYPE_KINGSIDE_CASTLING,
    MOVE_VALUE_TYPE_QUEENSIDE_CASTLING,
    MOVE_VALUE_TYPE_PROMOTION,
};

TEST(MoveValueTest, GivenAllValues_ExpectEveryValueIsUnique)
{
    auto all_move_values = ALL_MOVE_VALUES;  // non-const copy for unique
    std::array<Bitmove, 9>::iterator new_end_it = std::unique(all_move_values.begin(), all_move_values.end());
    const bool is_unique = new_end_it == all_move_values.end();
    EXPECT_TRUE(is_unique);
}

class MoveValueFixture : public ::testing::TestWithParam<Bitmove>
{
};

TEST_P(MoveValueFixture, GivenValue_ExpectIsEnirelyWithinBoundsOfMask)
{
    const Bitmove move_value_type = GetParam();
    EXPECT_EQ(move_value_type, move_value_type & MOVE_MASK_TYPE);
}

INSTANTIATE_TEST_SUITE_P(AllElements, MoveValueFixture, ::testing::ValuesIn(ALL_MOVE_VALUES));

// Shift tests
TEST(MoveShiftTargetTest, GivenOnAllOnes_Expect63)
{
    EXPECT_EQ((MOVE_ONES & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET, 0b00111111);
}

TEST(MoveShiftCapturedPieceTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((MOVE_ONES & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE, 0b00000111);
}

TEST(MoveShiftMovedPieceTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((MOVE_ONES & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE, 0b00000111);
}

TEST(MoveShiftPromotionTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((MOVE_ONES & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION, 0b00000111);
}

TEST(MoveShiftTypeTest, GivenOnAllOnes_Expect15)
{
    EXPECT_EQ((MOVE_ONES & MOVE_MASK_TYPE) >> MOVE_SHIFT_TYPE, 0b00001111);
}

}  // namespace
}  // namespace Chess
