#include "bitboard/uci_conversion.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

TEST(ToUciString, GivenEmptyBitmove_ExpectUciNullmove)
{
    EXPECT_EQ(ToUciString(kBitNullMove), kUciNullMove);
}

TEST(ToUciString, GivenBitmove_ExpectCorrectUciMove)
{
    const Bitmove arbritrary_move =
        ComposeMove(tzcnt(E7), tzcnt(E1), kRook, kNoCapture, kNoPromotion, kMoveTypeQuietNonPawn);
    EXPECT_EQ(ToUciString(arbritrary_move), "e7e1");
}

TEST(ToUciString, GivenBitmoveWithPromotion_ExpectCorrectUciMove)
{
    const Bitmove arbritrary_move =
        ComposeMove(tzcnt(A2), tzcnt(A1), kPawn, kNoCapture, kKnight, kMoveTypeQuietNonPawn);
    EXPECT_EQ(ToUciString(arbritrary_move), "a2a1n");
}

}  // namespace
}  // namespace Chess