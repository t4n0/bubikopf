#include "bitboard/move.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>

namespace Chess
{
namespace
{

constexpr std::array<Bitmove, 7> ALL_MOVE_MASKS{
    kMoveMaskSource,
    kMoveMaskTarget,
    kMoveMaskMovedPiece,
    kMoveMaskCapturedPiece,
    kMoveMaskPromotion,
    kMoveMaskType,
    kMoveMaskUnused,
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
    kMoveTypeQuietNonPawn,
    kMoveTypeCapture,
    kMoveTypePawnSinglePush,
    kMoveTypePawnDoublePush,
    kMoveTypeEnPassantCapture,
    kMoveTypeKingsideCastling,
    kMoveTypeQueensideCastling,
    kMoveTypePromotion,
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
    EXPECT_EQ(move_value_type, move_value_type & kMoveMaskType);
}

INSTANTIATE_TEST_SUITE_P(AllElements, MoveValueFixture, ::testing::ValuesIn(ALL_MOVE_VALUES));

constexpr Bitmove kAllBitsSet = std::numeric_limits<Bitmove>::max();

TEST(MoveShiftTargetTest, GivenOnAllOnes_Expect63)
{
    EXPECT_EQ((kAllBitsSet & kMoveMaskTarget) >> kMoveShiftTarget, 0b00111111);
}

TEST(MoveShiftCapturedPieceTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((kAllBitsSet & kMoveMaskCapturedPiece) >> kMoveShiftCapturedPiece, 0b00000111);
}

TEST(MoveShiftMovedPieceTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((kAllBitsSet & kMoveMaskMovedPiece) >> kMoveShiftMovedPiece, 0b00000111);
}

TEST(MoveShiftPromotionTest, GivenOnAllOnes_Expect7)
{
    EXPECT_EQ((kAllBitsSet & kMoveMaskPromotion) >> kMoveShiftPromotion, 0b00000111);
}

TEST(MoveShiftTypeTest, GivenOnAllOnes_Expect15)
{
    EXPECT_EQ((kAllBitsSet & kMoveMaskType) >> kMoveShiftType, 0b00001111);
}

TEST(ToUciString, GivenEmptyBitmove_ExpectUciNullmove)
{
    EXPECT_EQ(ToUciString(Bitmove{0}), kUciNullMove);
}

TEST(ToUciString, GivenBitmove_ExpectCorrectUciMove)
{
    const Bitmove arbritrary_move =
        ComposeMove(tzcnt(E7), tzcnt(E1), ROOK, kNoCapture, kNoPromotion, kMoveTypeQuietNonPawn);
    EXPECT_EQ(ToUciString(arbritrary_move), "e7e1");
}

TEST(ToUciString, GivenBitmoveWithPromotion_ExpectCorrectUciMove)
{
    const Bitmove arbritrary_move = ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, kNoCapture, KNIGHT, kMoveTypeQuietNonPawn);
    EXPECT_EQ(ToUciString(arbritrary_move), "a2a1n");
}

}  // namespace
}  // namespace Chess
