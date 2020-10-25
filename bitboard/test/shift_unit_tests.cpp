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

const SingleStepTestParameter A1_all_directions{A1, {XX, XX, A2, B2, B1, XX, XX, XX}};
const SingleStepTestParameter A4_all_directions{A4, {XX, XX, A5, B5, B4, B3, A3, XX}};
const SingleStepTestParameter A8_all_directions{A8, {XX, XX, XX, XX, B8, B7, A7, XX}};
const SingleStepTestParameter H1_all_directions{H1, {G1, G2, H2, XX, XX, XX, XX, XX}};
const SingleStepTestParameter H5_all_directions{H5, {G5, G6, H6, XX, XX, XX, H4, G4}};
const SingleStepTestParameter H8_all_directions{H8, {G8, XX, XX, XX, XX, XX, H7, G7}};
const SingleStepTestParameter E1_all_directions{E1, {D1, D2, E2, F2, F1, XX, XX, XX}};
const SingleStepTestParameter E4_all_directions{E4, {D4, D5, E5, F5, F4, F3, E3, D3}};
const SingleStepTestParameter D8_all_directions{D8, {C8, XX, XX, XX, E8, E7, D7, C7}};

INSTANTIATE_TEST_SUITE_P(AllCornersAndDirections,
                         SingleStepTestFixture,
                         ::testing::ValuesIn({
                             A1_all_directions,
                             A4_all_directions,
                             A8_all_directions,
                             H1_all_directions,
                             H5_all_directions,
                             H8_all_directions,
                             E1_all_directions,
                             E4_all_directions,
                             D8_all_directions,
                         }));

}  // namespace
}  // namespace Chess