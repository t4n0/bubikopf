#include "bitboard/fen_conversion.h"
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
    unit.boards_[kExtrasBoard] = 1;
    unit.boards_[kBlackBoard] = 2;
    unit.boards_[kBlackBoard + kPawn] = 3;
    unit.boards_[kBlackBoard + kKnight] = 4;
    unit.boards_[kBlackBoard + kBishop] = 5;
    unit.boards_[kBlackBoard + kRook] = 6;
    unit.boards_[kBlackBoard + kQueen] = 7;
    unit.boards_[kBlackBoard + kKing] = 8;
    unit.boards_[kUnused1Board] = 9;
    unit.boards_[kWhiteBoard] = 10;
    unit.boards_[kWhiteBoard + kPawn] = 11;
    unit.boards_[kWhiteBoard + kKnight] = 12;
    unit.boards_[kWhiteBoard + kBishop] = 13;
    unit.boards_[kWhiteBoard + kRook] = 14;
    unit.boards_[kWhiteBoard + kQueen] = 15;
    unit.boards_[kWhiteBoard + kKing] = 16;
    unit.boards_[kUnused2Board] = 17;

    Bitboard expected_unique_number = 1;
    for (Bitboard board : unit.boards_)
    {
        EXPECT_EQ(board, expected_unique_number);
        expected_unique_number++;
    }
}

TEST(GetTotalPliesTest, GivenEmptyPosition_ExpectNoPliesPlayed)
{
    const Position position{};
    constexpr std::size_t expected_number_of_plies_played = 0;
    EXPECT_EQ(position.GetTotalPlies(), expected_number_of_plies_played);
}

TEST(GetTotalPliesTest, GivenStandardStartingPosition_ExpectNoPliesPlayed)
{
    const Position position = PositionFromFen(kStandardStartingPosition);
    constexpr std::size_t expected_number_of_plies_played = 0;
    EXPECT_EQ(position.GetTotalPlies(), expected_number_of_plies_played);
}

TEST(GetTotalPliesTest, GivenPositionAfter6Plies_Expect6Plies)
{
    const Position position = PositionFromFen("rnbqk1nr/ppp1bppp/4p3/3p4/3P4/4PN2/PPP2PPP/RNBQKB1R w KQkq - 2 4");
    constexpr std::size_t expected_number_of_plies_played = 6;
    EXPECT_EQ(position.GetTotalPlies(), expected_number_of_plies_played);
}

TEST(MakeUnmakeMoveTest, GivenArbitraryMove_ExpectTotalPliesCountIncremented)
{
    const std::string arbitrary_fen = "r5k1/1p4pp/1p2p3/2P1prq1/1P2Q3/P4PPn/1B5P/R4RK1 w - - 1 24";
    constexpr Bitmove arbitrary_move{};

    Position position = PositionFromFen(arbitrary_fen);
    const auto total_plies_original = position.GetTotalPlies();

    const Bitboard saved_extras = position.MakeMove(arbitrary_move);
    const auto total_plies_after_make = position.GetTotalPlies();

    EXPECT_EQ(total_plies_original + 1, total_plies_after_make);

    position.UnmakeMove(arbitrary_move, saved_extras);
    const auto total_plies_after_unmake = position.GetTotalPlies();

    EXPECT_EQ(total_plies_original, total_plies_after_unmake);
}

}  // namespace
}  // namespace Chess
