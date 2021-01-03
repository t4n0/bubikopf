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
    std::cout << GetFen() << std::endl;
    PrettyPrintFen(fen_after_round_trip_conversion);
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
    position[kExtrasBoard] = kBoardMaskCastling;
    position[kExtrasBoard] |= kOneFullMove;
    position.white_to_move_ = true;

    // white pieces
    position[kWhiteBoard] = kRank2 | kRank1;
    position[kWhiteBoard + kPawn] = kRank2;
    position[kWhiteBoard + kRook] = A1 | H1;
    position[kWhiteBoard + kKnight] = B1 | G1;
    position[kWhiteBoard + kBishop] = C1 | F1;
    position[kWhiteBoard + kQueen] = D1;
    position[kWhiteBoard + kKing] = E1;

    // black pieces
    position[kBlackBoard] = kRank7 | kRank8;
    position[kBlackBoard + kPawn] = kRank7;
    position[kBlackBoard + kRook] = A8 | H8;
    position[kBlackBoard + kKnight] = B8 | G8;
    position[kBlackBoard + kBishop] = C8 | F8;
    position[kBlackBoard + kQueen] = D8;
    position[kBlackBoard + kKing] = E8;

    return position;
}

TEST(PositionFromFenTestFixture, StandardPosition)
{
    const auto position = PositionFromFen(kStandardStartingPosition);
    EXPECT_EQ(position, StandardStartingPosition());
}

}  // namespace
}  // namespace Chess