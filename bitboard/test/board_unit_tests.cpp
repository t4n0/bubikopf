#include "bitboard/board.h"

#include <gtest/gtest.h>

#include <limits>

namespace Chess
{
namespace
{

constexpr std::array<Bitboard, 5> kAllBoardMasks{
    kBoardMaskStaticPlies,
    kBoardMaskEnPassant,
    kBoardMaskCastling,
    kBoardMaskFullMoves,
    kBoardMaskUnused,
};

TEST(BoardMaskTest, GivenAllBoardMasks_ExpectEntireRangeOfUnderlyingTypeIsUtilized)
{
    Bitboard covered_fields = 0;
    for (const Bitboard mask : kAllBoardMasks)
    {
        covered_fields |= mask;
    }
    EXPECT_EQ(covered_fields, std::numeric_limits<Bitboard>::max());
}

class BoardMaskFixture : public ::testing::TestWithParam<std::tuple<Bitboard, Bitboard>>
{
};

TEST_P(BoardMaskFixture, GivenTwoDifferentMoveMasks_ExpectNoOverlap)
{
    const Bitboard first = std::get<0>(GetParam());
    const Bitboard second = std::get<1>(GetParam());
    if (first != second)
    {  // only check permutations with different masks
        EXPECT_FALSE(first & second);
    }
}

INSTANTIATE_TEST_SUITE_P(AllCombinations,
                         BoardMaskFixture,
                         ::testing::Combine(::testing::ValuesIn(kAllBoardMasks), ::testing::ValuesIn(kAllBoardMasks)));

constexpr Bitboard kAllBitsSet = std::numeric_limits<Bitboard>::max();

TEST(BoardShiftEnPassantTest, GivenOnAllOnes_Expect6BitsSet)
{
    EXPECT_EQ((kAllBitsSet & kBoardMaskEnPassant) >> kShiftEnPassant, 0b00111111);
}

TEST(BoardShiftFullMovesTest, GivenOnAllOnes_Expect8BitsSet)
{
    EXPECT_EQ((kAllBitsSet & kBoardMaskFullMoves) >> kShiftFullMoves, 0b11111111);
}

}  // namespace
}  // namespace Chess
