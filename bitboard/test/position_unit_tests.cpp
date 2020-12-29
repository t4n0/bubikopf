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

TEST(MakeUnmakeMoveTest, GivenWhiteMove_ExpectMoveCountUnchanged)
{
    const std::string arbitrary_fen = "r5k1/1p4pp/1p2p3/2P1prq1/1P2Q3/P4PPn/1B5P/R4RK1 w - - 1 24";
    const Bitmove arbitrary_move{};

    Position position = PositionFromFen(arbitrary_fen);
    position.MakeMove(arbitrary_move);

    const auto moves_original = std::stoi(TokenizeFen(arbitrary_fen).at(kFenTokenMoves));
    const auto moves_after_make = std::stoi(TokenizeFen(FenFromPosition(position)).at(kFenTokenMoves));
    EXPECT_EQ(moves_original, moves_after_make);

    position.UnmakeMove(arbitrary_move);

    const auto moves_after_unmake = std::stoi(TokenizeFen(FenFromPosition(position)).at(kFenTokenMoves));
    EXPECT_EQ(moves_original, moves_after_unmake);
}

TEST(MakeUnmakeMoveTest, GivenBlackMove_ExpectMoveCountIncremented)
{
    const std::string arbitrary_fen = "r5k1/1p4pp/1p2p3/2P1prq1/1P2Q3/P4PPn/1B5P/R4R1K b - - 2 24";
    const Bitmove arbitrary_move{};

    Position position = PositionFromFen(arbitrary_fen);
    position.MakeMove(arbitrary_move);

    const auto moves_original = std::stoi(TokenizeFen(arbitrary_fen).at(kFenTokenMoves));
    const auto moves_after_make = std::stoi(TokenizeFen(FenFromPosition(position)).at(kFenTokenMoves));
    EXPECT_EQ(moves_original + 1, moves_after_make);

    position.UnmakeMove(arbitrary_move);

    const auto moves_after_unmake = std::stoi(TokenizeFen(FenFromPosition(position)).at(kFenTokenMoves));
    EXPECT_EQ(moves_original, moves_after_unmake);
}

TEST(MakeUnmakeMoveTest, GivenStartPosition_ExpectNoMovesPlayed)
{
    const Position position = PositionFromFen(kStandardStartingPosition);
    const std::size_t expected_number_of_plies_played = 0;
    EXPECT_EQ(position.GetNumberOfPlayedPlies(), expected_number_of_plies_played);
}

TEST(MakeUnmakeMoveTest, GivenPositionAfter3FullMovesAndWhiteToPlay_Expect6Plies)
{
    const Position position = PositionFromFen("rnbqk1nr/ppp1bppp/4p3/3p4/3P4/4PN2/PPP2PPP/RNBQKB1R w KQkq - 2 4");
    const std::size_t expected_number_of_plies_played = 6;
    EXPECT_EQ(position.GetNumberOfPlayedPlies(), expected_number_of_plies_played);
}

TEST(MakeUnmakeMoveTest, GivenPositionAfter3FullMovesAndBlackToPlay_Expect7Plies)
{
    const Position position = PositionFromFen("rnbqk1nr/ppp1bppp/4p3/3p4/3P4/2P1PN2/PP3PPP/RNBQKB1R b KQkq - 0 4");
    const std::size_t expected_number_of_plies_played = 7;
    EXPECT_EQ(position.GetNumberOfPlayedPlies(), expected_number_of_plies_played);
}

}  // namespace
}  // namespace Chess
