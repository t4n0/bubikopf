#include "bitboard/squares.h"

#include <gmock/gmock.h>

#include <vector>
#include <limits>

namespace Chess
{
namespace
{

struct SquareTestParameter
{
    std::vector<Bitboard> elements;
    int expected_size;
};

class SquareTestFixture : public ::testing::TestWithParam<SquareTestParameter>
{
};

TEST_P(SquareTestFixture, GivenXorOverAllSquaers_ExpectUnderlyingTypeIsMax)
{
    EXPECT_THAT(GetParam().elements.size(), GetParam().expected_size);

    Bitboard xor_over_all_elements = 0;
    Bitboard last_iteration = 0;
    for (const Bitboard element : GetParam().elements)
    {
        xor_over_all_elements ^= element;
        EXPECT_FALSE(last_iteration & ~xor_over_all_elements);  // Not bit was unset in this iteration
        last_iteration = xor_over_all_elements;
    }
    EXPECT_EQ(xor_over_all_elements, std::numeric_limits<Bitboard>::max());
}

const SquareTestParameter ALL_SQUARES{
    {H1, G1, F1, E1, D1, C1, B1, A1, H2, G2, F2, E2, D2, C2, B2, A2, H3, G3, F3, E3, D3, C3,
     B3, A3, H4, G4, F4, E4, D4, C4, B4, A4, H5, G5, F5, E5, D5, C5, B5, A5, H6, G6, F6, E6,
     D6, C6, B6, A6, H7, G7, F7, E7, D7, C7, B7, A7, H8, G8, F8, E8, D8, C8, B8, A8},
    64};
const SquareTestParameter ALL_RANKS{{RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8}, 8};
const SquareTestParameter ALL_FILES{{FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H}, 8};

INSTANTIATE_TEST_SUITE_P(AllSquares, SquareTestFixture, ::testing::ValuesIn({ALL_SQUARES}));
INSTANTIATE_TEST_SUITE_P(AllRanks, SquareTestFixture, ::testing::ValuesIn({ALL_RANKS}));
INSTANTIATE_TEST_SUITE_P(AllFiles, SquareTestFixture, ::testing::ValuesIn({ALL_FILES}));

}  // namespace
}  // namespace Chess
