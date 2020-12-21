#include "bitboard/position.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <gmock/gmock.h>

#include <algorithm>
#include <iterator>

namespace Chess
{
namespace
{

TEST(BoardIndxesTest, GivenUniqueNumberToEachBitboard_ExpectRespectiveUniqueNumber)
{
    Position unit{};
    unit.boards_[BOARD_IDX_EXTRAS] = 1;
    unit.boards_[BOARD_IDX_BLACK] = 2;
    unit.boards_[BOARD_IDX_BLACK + PAWN] = 3;
    unit.boards_[BOARD_IDX_BLACK + KNIGHT] = 4;
    unit.boards_[BOARD_IDX_BLACK + BISHOP] = 5;
    unit.boards_[BOARD_IDX_BLACK + ROOK] = 6;
    unit.boards_[BOARD_IDX_BLACK + QUEEN] = 7;
    unit.boards_[BOARD_IDX_BLACK + KING] = 8;
    unit.boards_[BOARD_IDX_UNUSED1] = 9;
    unit.boards_[BOARD_IDX_WHITE] = 10;
    unit.boards_[BOARD_IDX_WHITE + PAWN] = 11;
    unit.boards_[BOARD_IDX_WHITE + KNIGHT] = 12;
    unit.boards_[BOARD_IDX_WHITE + BISHOP] = 13;
    unit.boards_[BOARD_IDX_WHITE + ROOK] = 14;
    unit.boards_[BOARD_IDX_WHITE + QUEEN] = 15;
    unit.boards_[BOARD_IDX_WHITE + KING] = 16;
    unit.boards_[BOARD_IDX_UNUSED2] = 17;

    Bitboard expected_unique_number = 1;
    for (Bitboard board : unit.boards_)
    {
        EXPECT_EQ(board, expected_unique_number);
        expected_unique_number++;
    }
}

}  // namespace
}  // namespace Chess
