#include "bitboard/move.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <limits>

namespace Chess
{
namespace
{

constexpr std::array<Bitmove, 7> kAllMoveMasks{
    kMoveMaskSource,
    kMoveMaskTarget,
    kMoveMaskMovedPiece,
    kMoveMaskCapturedPiece,
    kMoveMaskPromotion,
    kMoveMaskType,
    kMoveMaskUnused,
};

TEST(MoveMaskTest, GivenAllMoveMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized)
{
    Bitmove covered_fields = 0;
    for (const Bitmove mask : kAllMoveMasks)
    {
        covered_fields |= mask;
    }
    EXPECT_EQ(covered_fields, std::numeric_limits<Bitmove>::max());
}

class MoveMaskFixture : public testing::TestWithParam<std::tuple<Bitmove, Bitmove>>
{
};

TEST_P(MoveMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap)
{
    const Bitmove first = std::get<0>(GetParam());
    const Bitmove second = std::get<1>(GetParam());
    if (first != second)  // only check combinations with different masks
    {
        EXPECT_FALSE(first & second);
    }
}

INSTANTIATE_TEST_SUITE_P(AllCombinations,
                         MoveMaskFixture,
                         testing::Combine(testing::ValuesIn(kAllMoveMasks), testing::ValuesIn(kAllMoveMasks)));

const std::array<Bitmove, 8> kAllMoveValues{
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
    auto all_move_values = kAllMoveValues;  // non-const copy for unique
    std::sort(all_move_values.begin(), all_move_values.end());
    std::array<Bitmove, 9>::iterator new_end_it = std::unique(all_move_values.begin(), all_move_values.end());
    const bool is_unique = new_end_it == all_move_values.end();
    EXPECT_TRUE(is_unique);
}

class MoveValueFixture : public testing::TestWithParam<Bitmove>
{
};

TEST_P(MoveValueFixture, GivenValue_ExpectIsEnirelyWithinBoundsOfMask)
{
    const Bitmove move_value_type = GetParam();
    EXPECT_EQ(move_value_type, move_value_type & kMoveMaskType);
}

INSTANTIATE_TEST_SUITE_P(AllElements, MoveValueFixture, testing::ValuesIn(kAllMoveValues));

constexpr Bitmove kAllBitsSet = std::numeric_limits<Bitmove>::max();
constexpr Bitmove k6BitsSet = 0b00111111;
constexpr Bitmove k4BitsSet = 0b00001111;
constexpr Bitmove k3BitsSet = 0b00000111;

TEST(ExtractSourceTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractSource(kAllBitsSet), k6BitsSet);
}

TEST(ExtractTargetTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractTarget(kAllBitsSet), k6BitsSet);
}

TEST(ExtractMovedPieceTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractMovedPiece(kAllBitsSet), k3BitsSet);
}

TEST(ExtractCapturedPieceTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractCapturedPiece(kAllBitsSet), k3BitsSet);
}

TEST(ExtractPromotionTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractPromotion(kAllBitsSet), k3BitsSet);
}

TEST(ExtractTypeTest, GivenAllBitsSet_WidthOfRespectiveMaskCovered)
{
    EXPECT_EQ(ExtractType(kAllBitsSet), k4BitsSet);
}

}  // namespace
}  // namespace Chess
