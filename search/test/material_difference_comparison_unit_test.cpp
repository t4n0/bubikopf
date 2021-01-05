#include "search/material_difference_comparison.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{
Bitmove ComposeMoveWithMaterial(const Bitmove captured_piece, const Bitmove promotion)
{
    constexpr Bitmove arbitrary_source{0};
    constexpr Bitmove arbitrary_target{1};
    constexpr Bitmove arbitrary_moved_piece{2};
    constexpr Bitmove arbitrary_type{3};
    return ComposeMove(
        arbitrary_source, arbitrary_target, arbitrary_moved_piece, captured_piece, promotion, arbitrary_type);
}

struct MaterialDifferenceIsGreaterTestFixture : public testing::TestWithParam<std::tuple<Bitmove, Bitmove>>
{
    Bitmove GetBetterMove() { return std::get<0>(GetParam()); }
    Bitmove GetWorseMove() { return std::get<1>(GetParam()); }
};

TEST_P(MaterialDifferenceIsGreaterTestFixture, GivenBetterOptionFirst_ExpectTrue)
{
    EXPECT_TRUE(IsMaterialDifferenceGreater(GetBetterMove(), GetWorseMove()));
}

const std::array<std::tuple<Bitmove, Bitmove>, 6> kPairsOfMovesWithBetterOneFirst{{
    {ComposeMoveWithMaterial(kPawn, kNoPromotion), ComposeMoveWithMaterial(kNoCapture, kNoPromotion)},
    {ComposeMoveWithMaterial(kKnight, kNoPromotion), ComposeMoveWithMaterial(kPawn, kNoPromotion)},
    {ComposeMoveWithMaterial(kNoCapture, kQueen), ComposeMoveWithMaterial(kRook, kNoPromotion)},
    {ComposeMoveWithMaterial(kQueen, kNoPromotion), ComposeMoveWithMaterial(kNoCapture, kQueen)},
    {ComposeMoveWithMaterial(kNoCapture, kRook), ComposeMoveWithMaterial(kBishop, kNoPromotion)},
    {ComposeMoveWithMaterial(kRook, kRook), ComposeMoveWithMaterial(kNoCapture, kQueen)},
}};

INSTANTIATE_TEST_SUITE_P(PairsOfMovesWithBetterOptionFirst,
                         MaterialDifferenceIsGreaterTestFixture,
                         testing::ValuesIn(kPairsOfMovesWithBetterOneFirst));

struct MatterialDifferenceIsNullOrWorseTestFixture : public testing::TestWithParam<std::tuple<Bitmove, Bitmove>>
{
    Bitmove GetFirstMove() { return std::get<0>(GetParam()); }
    Bitmove GetSecondMove() { return std::get<1>(GetParam()); }
};

TEST_P(MatterialDifferenceIsNullOrWorseTestFixture, GivenSimilarOrWorseOptionFirst_ExpectFalse)
{
    EXPECT_FALSE(IsMaterialDifferenceGreater(GetFirstMove(), GetSecondMove()));
}

const std::array<std::tuple<Bitmove, Bitmove>, 3> kPairsOfMovesWithEqualOptions{{
    {ComposeMoveWithMaterial(kNoCapture, kNoPromotion), ComposeMoveWithMaterial(kNoCapture, kNoPromotion)},
    {ComposeMoveWithMaterial(kPawn, kNoPromotion), ComposeMoveWithMaterial(kPawn, kNoPromotion)},
    {ComposeMoveWithMaterial(kNoCapture, kQueen), ComposeMoveWithMaterial(kNoCapture, kQueen)},
}};

INSTANTIATE_TEST_SUITE_P(PairsOfMovesWithEqualOptions,
                         MatterialDifferenceIsNullOrWorseTestFixture,
                         testing::ValuesIn(kPairsOfMovesWithEqualOptions));

const std::array<std::tuple<Bitmove, Bitmove>, 2> kPairsOfMovesWithWorseOptionFirst{{
    {ComposeMoveWithMaterial(kRook, kNoPromotion), ComposeMoveWithMaterial(kQueen, kNoPromotion)},
    {ComposeMoveWithMaterial(kNoCapture, kKnight), ComposeMoveWithMaterial(kNoCapture, kQueen)},
}};

INSTANTIATE_TEST_SUITE_P(PairsOfMovesWithWorstOptionFirst,
                         MatterialDifferenceIsNullOrWorseTestFixture,
                         testing::ValuesIn(kPairsOfMovesWithWorseOptionFirst));

TEST(SortWithRespectToMaterialDifference, GivenUnorderdMoveStack_ExpectCorrectOrder)
{
    // Setup
    const Bitmove best_move = ComposeMoveWithMaterial(kRook, kQueen);
    const Bitmove second_best_move = ComposeMoveWithMaterial(kQueen, kNoPromotion);
    const Bitmove middle_move = ComposeMoveWithMaterial(kNoCapture, kRook);
    const Bitmove second_to_worst_move = ComposeMoveWithMaterial(kPawn, kNoPromotion);
    const Bitmove worst_move = ComposeMoveWithMaterial(kNoCapture, kNoPromotion);
    std::array<Bitmove, 5> move_stack{second_to_worst_move, best_move, worst_move, second_best_move, middle_move};

    // Call
    std::sort(move_stack.begin(), move_stack.end(), IsMaterialDifferenceGreater);

    // Expect
    const std::array<Bitmove, 5> expected_stack_order{
        best_move,
        second_best_move,
        middle_move,
        second_to_worst_move,
        worst_move,
    };

    EXPECT_EQ(move_stack, expected_stack_order);
}

}  // namespace
}  // namespace Chess
