#include "search/principal_variation.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(ClearSublines, GivenCall_ExpectMainLineUnalteredAndSublinesCleared)
{
    // Setup
    PrincipalVariation principal_variation{};
    constexpr Bitmove arbirtary_move{42};
    principal_variation.fill(arbirtary_move);

    // Call
    ClearSublines(principal_variation);

    // Expect
    for (std::size_t index{0}; index < principal_variation.size(); index++)
    {
        if (index < GetSublineIndexAtDepth(1))
        {
            EXPECT_EQ(principal_variation[index], arbirtary_move);
        }
        else
        {
            EXPECT_EQ(principal_variation[index], kBitNullMove);
        }
    }
}

}  // namespace
}  // namespace Chess
