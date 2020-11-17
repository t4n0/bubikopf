#include "bitboard/lookup_table/knight.h"
#include "bitboard/shift.h"
#include "bitboard/squares.h"

#include <gmock/gmock.h>

namespace Chess
{
namespace
{

struct SingleStepTestParameter
{
    Bitboard source;
    std::array<Bitboard, 8> expected_targets_per_direction;
};

class SingleStepTestFixture : public ::testing::TestWithParam<SingleStepTestParameter>
{
};

TEST_P(SingleStepTestFixture, GivenSquare_ExpectOnlyValidTargetsForAllDirections)
{
    const std::array<Bitboard, 8>& expected_targets = GetParam().expected_targets_per_direction;
    ASSERT_THAT(SingleStep(GetParam().source, west), ::testing::Eq(expected_targets.at(0)));
    ASSERT_THAT(SingleStep(GetParam().source, north_west), ::testing::Eq(expected_targets.at(1)));
    ASSERT_THAT(SingleStep(GetParam().source, north), ::testing::Eq(expected_targets.at(2)));
    ASSERT_THAT(SingleStep(GetParam().source, north_east), ::testing::Eq(expected_targets.at(3)));
    ASSERT_THAT(SingleStep(GetParam().source, east), ::testing::Eq(expected_targets.at(4)));
    ASSERT_THAT(SingleStep(GetParam().source, south_east), ::testing::Eq(expected_targets.at(5)));
    ASSERT_THAT(SingleStep(GetParam().source, south), ::testing::Eq(expected_targets.at(6)));
    ASSERT_THAT(SingleStep(GetParam().source, south_west), ::testing::Eq(expected_targets.at(7)));
}

const SingleStepTestParameter A1_step_in_all_directions{A1, {XX, XX, A2, B2, B1, XX, XX, XX}};
const SingleStepTestParameter A4_step_in_all_directions{A4, {XX, XX, A5, B5, B4, B3, A3, XX}};
const SingleStepTestParameter A8_step_in_all_directions{A8, {XX, XX, XX, XX, B8, B7, A7, XX}};
const SingleStepTestParameter H1_step_in_all_directions{H1, {G1, G2, H2, XX, XX, XX, XX, XX}};
const SingleStepTestParameter H5_step_in_all_directions{H5, {G5, G6, H6, XX, XX, XX, H4, G4}};
const SingleStepTestParameter H8_step_in_all_directions{H8, {G8, XX, XX, XX, XX, XX, H7, G7}};
const SingleStepTestParameter E1_step_in_all_directions{E1, {D1, D2, E2, F2, F1, XX, XX, XX}};
const SingleStepTestParameter E4_step_in_all_directions{E4, {D4, D5, E5, F5, F4, F3, E3, D3}};
const SingleStepTestParameter D8_step_in_all_directions{D8, {C8, XX, XX, XX, E8, E7, D7, C7}};

INSTANTIATE_TEST_SUITE_P(AllCornersAndDirections,
                         SingleStepTestFixture,
                         ::testing::Values(A1_step_in_all_directions,
                                           A4_step_in_all_directions,
                                           A8_step_in_all_directions,
                                           H1_step_in_all_directions,
                                           H5_step_in_all_directions,
                                           H8_step_in_all_directions,
                                           E1_step_in_all_directions,
                                           E4_step_in_all_directions,
                                           D8_step_in_all_directions));

struct KnightJumpTestParameter
{
    Bitboard source;
    std::array<Bitboard, 8> expected_targets_per_direction;
};

class KnightJumpTestFixture : public ::testing::TestWithParam<KnightJumpTestParameter>
{
};

TEST_P(KnightJumpTestFixture, GivenSquare_ExpectOnlyValidTargetsForAllDirections)
{
    const std::array<Bitboard, 8>& expected_targets = GetParam().expected_targets_per_direction;
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_north_west), ::testing::Eq(expected_targets.at(0)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_north_east), ::testing::Eq(expected_targets.at(1)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_east_north), ::testing::Eq(expected_targets.at(2)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_east_south), ::testing::Eq(expected_targets.at(3)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_south_east), ::testing::Eq(expected_targets.at(4)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_south_west), ::testing::Eq(expected_targets.at(5)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_west_south), ::testing::Eq(expected_targets.at(6)));
    ASSERT_THAT(RuntimeKnightJump(GetParam().source, knight_west_north), ::testing::Eq(expected_targets.at(7)));
}

const KnightJumpTestParameter B7_jump_in_all_directions{B7, {XX, XX, D8, D6, C5, A5, XX, XX}};
const KnightJumpTestParameter C7_jump_in_all_directions{C7, {XX, XX, E8, E6, D5, B5, A6, A8}};
const KnightJumpTestParameter B6_jump_in_all_directions{B6, {A8, C8, D7, D5, C4, A4, XX, XX}};
const KnightJumpTestParameter C6_jump_in_all_directions{C6, {B8, D8, E7, E5, D4, B4, A5, A7}};
const KnightJumpTestParameter F3_jump_in_all_directions{F3, {E5, G5, H4, H2, G1, E1, D2, D4}};
const KnightJumpTestParameter G3_jump_in_all_directions{G3, {F5, H5, XX, XX, H1, F1, E2, E4}};
const KnightJumpTestParameter F2_jump_in_all_directions{F2, {E4, G4, H3, H1, XX, XX, D1, D3}};
const KnightJumpTestParameter G2_jump_in_all_directions{G2, {F4, H4, XX, XX, XX, XX, E1, E3}};
const KnightJumpTestParameter A1_jump_in_all_directions{A1, {XX, B3, C2, XX, XX, XX, XX, XX}};
const KnightJumpTestParameter A8_jump_in_all_directions{A8, {XX, XX, XX, C7, B6, XX, XX, XX}};
const KnightJumpTestParameter H8_jump_in_all_directions{H8, {XX, XX, XX, XX, XX, G6, F7, XX}};
const KnightJumpTestParameter H1_jump_in_all_directions{H1, {G3, XX, XX, XX, XX, XX, XX, F2}};

INSTANTIATE_TEST_SUITE_P(VariousSquaresAndAllDirections,
                         KnightJumpTestFixture,
                         ::testing::Values(B7_jump_in_all_directions,
                                           C7_jump_in_all_directions,
                                           B6_jump_in_all_directions,
                                           C6_jump_in_all_directions,
                                           F3_jump_in_all_directions,
                                           G3_jump_in_all_directions,
                                           F2_jump_in_all_directions,
                                           G2_jump_in_all_directions,
                                           A1_jump_in_all_directions,
                                           A8_jump_in_all_directions,
                                           H8_jump_in_all_directions,
                                           H1_jump_in_all_directions));

}  // namespace
}  // namespace Chess