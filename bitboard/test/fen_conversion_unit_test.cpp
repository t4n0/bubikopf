#include "bitboard/board.h"
#include "bitboard/fen_conversion.h"
#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

namespace Chess
{
namespace
{

class RoundTripConversionTestFixture : public testing::TestWithParam<const char*>
{
  public:
    std::string GetFen() const { return GetParam(); }
};

TEST_P(RoundTripConversionTestFixture, GivenRoundTripConversion_ExpectOriginalFen)
{
    const auto position = PositionFromFen(GetFen());
    const auto fen_after_round_trip_conversion = FenFromPosition(position);

    EXPECT_EQ(GetFen(), fen_after_round_trip_conversion);
}

const std::array<const char*, 4> kArbitraryFens{
    "2r3k1/pp2npp1/3r2qp/8/2BBp3/1PP3Rb/P4P2/R2Q3K b - - 0 28",
    "8/8/1p5p/p2k2p1/PP4P1/1K5P/8/8 w - - 0 33",
    "8/p2r1pk1/1p3pp1/2p5/7Q/1B2B2P/Pq3P2/6K1 b - - 3 29",
    "r5k1/1p4pp/1p2p3/2P1prq1/1P2Q3/P4PPn/1B5P/R4RK1 w - - 1 24",
};

INSTANTIATE_TEST_SUITE_P(ArbitraryFens, RoundTripConversionTestFixture, testing::ValuesIn(kArbitraryFens));

class InvalidFenTestFixture : public testing::TestWithParam<std::string>
{
};

TEST_P(InvalidFenTestFixture, GivenInvalidFen_ExpectException)
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
                         InvalidFenTestFixture,
                         testing::Values(kMissingToken, kExtraToken, kSide, kCastling, kEnPassant));

Position StandardStartingPosition()
{
    Position position{};
    position[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING;
    position[BOARD_IDX_EXTRAS] |= BOARD_VALUE_ADD_MOVE;
    position.white_to_move_ = true;

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

TEST(PositionFromFenTestFixture, StandardPosition)
{
    const auto position = PositionFromFen(kStandardStartingPosition);
    EXPECT_EQ(position, StandardStartingPosition());
}

}  // namespace
}  // namespace Chess