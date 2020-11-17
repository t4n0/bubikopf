#include "bitboard/board.h"
#include "bitboard/position_from_fen.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

class PositionFromFenTestFixture : public ::testing::TestWithParam<std::string>
{
};

TEST_P(PositionFromFenTestFixture, GivenInvalidFen_ExpectException)
{
    EXPECT_THROW(PositionFromFen(GetParam()), std::runtime_error);
}

// invalid fens
const std::string kMissingToken = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 1";
const std::string kExtraToken = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 1 1 1";
const std::string kSide = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 B KQkq e3 1 1";
const std::string kCastling = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b KQkB e3 1 1";
const std::string kEnPassant = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b KQk e9 1 1";

INSTANTIATE_TEST_SUITE_P(InvalidFens,
                         PositionFromFenTestFixture,
                         ::testing::Values(kMissingToken, kExtraToken, kSide, kCastling, kEnPassant));

TEST_F(PositionFromFenTestFixture, WhiteToMove)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 w KQkq e3 1 1");
    EXPECT_TRUE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN);
}

TEST_F(PositionFromFenTestFixture, BlackToMove)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b KQkq e3 1 1");
    EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN);
}

TEST_F(PositionFromFenTestFixture, BlackKingsideCastling)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 w k e3 1 1");
    EXPECT_TRUE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_BLACK_KINGSIDE);
}

TEST_F(PositionFromFenTestFixture, BlackQueensideCastling)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 w q e3 1 1");
    EXPECT_TRUE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
}

TEST_F(PositionFromFenTestFixture, WhiteKingSideCastling)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b K e3 1 1");
    EXPECT_TRUE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_WHITE_KINGSIDE);
}

TEST_F(PositionFromFenTestFixture, WhiteQueenSideCastling)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b Q e3 1 1");
    EXPECT_TRUE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_WHITE_QUEENSIDE);
}

TEST_F(PositionFromFenTestFixture, NoCastling)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b - e3 1 1");
    EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_CASTLING);
}

TEST_F(PositionFromFenTestFixture, EnPassant)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b - e3 1 1");
    EXPECT_EQ((position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT) >> BOARD_SHIFT_EN_PASSANT, tzcnt(E3));
}

TEST_F(PositionFromFenTestFixture, NoEnPassant)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b - - 1 1");
    EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSANT);
}

TEST_F(PositionFromFenTestFixture, InvalidStaticMoveCount)
{
    EXPECT_THROW(PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b - - x 13"), std::invalid_argument);
}

TEST_F(PositionFromFenTestFixture, StaticMoveCount)
{
    const auto position = PositionFromFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/8 b - - 42 13");
    EXPECT_EQ((position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES), 42);
}

PositionWithBitboards StandardStartingPosition()
{
    PositionWithBitboards position{};
    position[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING | BOARD_MASK_WHITE_TURN;

    // white pieces
    position[BOARD_IDX_WHITE] = RANK_2 | RANK_1;
    position[BOARD_IDX_WHITE + PAWN] = RANK_2;
    position[BOARD_IDX_WHITE + ROOK] = A1 | H1;
    position[BOARD_IDX_WHITE + KNIGHT] = B1 | G1;
    position[BOARD_IDX_WHITE + BISHOP] = C1 | F1;
    position[BOARD_IDX_WHITE + QUEEN] = D1;
    position[BOARD_IDX_WHITE + KING] = E1;

    // black pieces
    position[BOARD_IDX_BLACK] = RANK_7 | RANK_8;
    position[BOARD_IDX_BLACK + PAWN] = RANK_7;
    position[BOARD_IDX_BLACK + ROOK] = A8 | H8;
    position[BOARD_IDX_BLACK + KNIGHT] = B8 | G8;
    position[BOARD_IDX_BLACK + BISHOP] = C8 | F8;
    position[BOARD_IDX_BLACK + QUEEN] = D8;
    position[BOARD_IDX_BLACK + KING] = E8;

    return position;
}

TEST_F(PositionFromFenTestFixture, StandardPosition)
{
    const auto position = PositionFromFen(kStandardStartingPosition);
    EXPECT_EQ(position, StandardStartingPosition());
}

}  // namespace
}  // namespace Chess