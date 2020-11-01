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
    PositionWithBitboards unit{};
    unit.boards_[BOARD_IDX_EXTRAS] = 1;
    unit.boards_[BOARD_IDX_BLACK] = 2;
    unit.boards_[BOARD_IDX_BLACK + PAWN] = 3;
    unit.boards_[BOARD_IDX_BLACK + KNIGHT] = 4;
    unit.boards_[BOARD_IDX_BLACK + BISHOP] = 5;
    unit.boards_[BOARD_IDX_BLACK + ROOK] = 6;
    unit.boards_[BOARD_IDX_BLACK + QUEEN] = 7;
    unit.boards_[BOARD_IDX_BLACK + KING] = 8;
    unit.boards_[BOARD_IDX_BLACK + BOARD_IDX_ATTACK] = 9;
    unit.boards_[BOARD_IDX_WHITE] = 10;
    unit.boards_[BOARD_IDX_WHITE + PAWN] = 11;
    unit.boards_[BOARD_IDX_WHITE + KNIGHT] = 12;
    unit.boards_[BOARD_IDX_WHITE + BISHOP] = 13;
    unit.boards_[BOARD_IDX_WHITE + ROOK] = 14;
    unit.boards_[BOARD_IDX_WHITE + QUEEN] = 15;
    unit.boards_[BOARD_IDX_WHITE + KING] = 16;
    unit.boards_[BOARD_IDX_WHITE + BOARD_IDX_ATTACK] = 17;

    Bitboard expected_unique_number = 1;
    for (Bitboard board : unit.boards_)
    {
        EXPECT_EQ(board, expected_unique_number);
        expected_unique_number++;
    }
}

TEST(PositionWhiteToMove, GivenWhitesTurn_ExpectTrue)
{
    // Setup
    PositionWithBitboards position{};
    position.boards_[BOARD_IDX_EXTRAS] = BOARD_ZEROS;
    position.boards_[BOARD_IDX_EXTRAS] |= BOARD_MASK_WHITE_TURN;

    // Call & Expect
    EXPECT_TRUE(position.WhiteToMove());
}

TEST(PositionWhiteToMove, GivenBlacksTurn_ExpectFalse)
{
    // Setup
    PositionWithBitboards position{};
    position.boards_[BOARD_IDX_EXTRAS] = BOARD_ONES;
    position.boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_WHITE_TURN;

    // Call & Expect
    EXPECT_FALSE(position.WhiteToMove());
}

TEST(MakeUnmakeMoveTest, GivenMove_ExpectHistoryIsAppended)
{
    // Setup
    const Bitmove arbitrary_move{};
    const Bitboard arbitrary_extras = 42;
    PositionWithBitboards position{};
    position.boards_[BOARD_IDX_EXTRAS] = arbitrary_extras;

    // Call
    position.MakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.extras_history_.at(0), arbitrary_extras);
    EXPECT_THAT(position.extras_history_insertion_index_, 1);

    // Call
    position.UnmakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.extras_history_insertion_index_, 0);
}

struct PriorSpecifier
{
    std::size_t index;
    Bitboard value;
};

struct PosteriorSpecifier
{
    std::size_t index;
    ::testing::Matcher<Bitboard> value;
};

struct MakeUnmakeMoveTestParameter
{
    std::vector<PriorSpecifier> prior_setup;
    Bitmove move;
    std::vector<PosteriorSpecifier> posterior_setup;
};

class MakeUnmakeMoveTestFixture : public ::testing::TestWithParam<MakeUnmakeMoveTestParameter>
{
};

TEST_P(MakeUnmakeMoveTestFixture, GivenMove_ExpectPosteriorAfterMakeAndPriorAfterUnmake)
{
    // Setup
    PositionWithBitboards position{};
    std::array<::testing::Matcher<Bitboard>, 17> expected_position_posterior{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    for (const auto& setter : GetParam().prior_setup)
    {
        position[setter.index] = setter.value;
    }
    for (const auto& setter : GetParam().posterior_setup)
    {
        expected_position_posterior[setter.index] = setter.value;
    }

    const PositionWithBitboards expected_position_prior{position};

    // Call
    position.MakeMove(GetParam().move);

    // Expect
    EXPECT_THAT(position.boards_, ::testing::ElementsAreArray(expected_position_posterior));

    // Call
    position.UnmakeMove(GetParam().move);

    // Expect
    EXPECT_THAT(position, expected_position_prior) << "Position does not equal prior after UnmakeMove(..).";
}

MATCHER(IsAnything, "")
{
    return true;
}

MATCHER(IsWhitesTurn, "")
{
    return arg & BOARD_MASK_WHITE_TURN;
}

MATCHER(HasEnPassantSet, "")
{
    return arg & BOARD_MASK_EN_PASSANT;
}

const MakeUnmakeMoveTestParameter kSideSwitchedFromWhiteToBlack{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_WHITE_TURN}},
    {},
    {{BOARD_IDX_EXTRAS, ::testing::Not(IsWhitesTurn())}},
};
const MakeUnmakeMoveTestParameter kSideSwitchedFromBlackToWhite{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_BLACK_TURN}},
    {},
    {{BOARD_IDX_EXTRAS, IsWhitesTurn()}},
};
const MakeUnmakeMoveTestParameter kEnPassantCleared{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_EN_PASSANT}},
    {},
    {{BOARD_IDX_EXTRAS, ::testing::Not(HasEnPassantSet())}},
};
const MakeUnmakeMoveTestParameter kBlackPieceMoved{
    {{BOARD_IDX_BLACK, A7}},
    {ComposeMove(tzcnt(A7), tzcnt(A6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 1 | BOARD_MASK_WHITE_TURN}, {BOARD_IDX_BLACK, A6}, {BOARD_IDX_BLACK + ROOK, IsAnything()}},
};
const MakeUnmakeMoveTestParameter kWhitePieceMoved{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, A7}},
    {ComposeMove(tzcnt(A7), tzcnt(A6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 1 | BOARD_MASK_BLACK_TURN}, {BOARD_IDX_WHITE, A6}, {BOARD_IDX_WHITE + ROOK, IsAnything()}},
};
const MakeUnmakeMoveTestParameter kQuietNonPawn{
    {{BOARD_IDX_EXTRAS, 2 | BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, H1}, {BOARD_IDX_WHITE + KNIGHT, H1}},
    {ComposeMove(tzcnt(H1), tzcnt(G3), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 3 | BOARD_MASK_BLACK_TURN}, {BOARD_IDX_WHITE, G3}, {BOARD_IDX_WHITE + KNIGHT, G3}},
};
const MakeUnmakeMoveTestParameter kCapture{
    {{BOARD_IDX_EXTRAS, 3 | BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_WHITE, D5},
     {BOARD_IDX_WHITE + PAWN, D5},
     {BOARD_IDX_BLACK, D1},
     {BOARD_IDX_BLACK + QUEEN, D1}},
    {ComposeMove(tzcnt(D1), tzcnt(D5), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_WHITE_TURN},
     {BOARD_IDX_WHITE, XX},
     {BOARD_IDX_WHITE + PAWN, XX},
     {BOARD_IDX_BLACK, D5},
     {BOARD_IDX_BLACK + QUEEN, D5}},
};
const MakeUnmakeMoveTestParameter kPawnSinglePush{
    {{BOARD_IDX_EXTRAS, 9 | BOARD_MASK_BLACK_TURN}, {BOARD_IDX_BLACK, F3}, {BOARD_IDX_BLACK + PAWN, F3}},
    {ComposeMove(tzcnt(F3), tzcnt(F2), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_WHITE_TURN}, {BOARD_IDX_BLACK, F2}, {BOARD_IDX_BLACK + PAWN, F2}},
};
const MakeUnmakeMoveTestParameter kPawnDoublePush{
    {{BOARD_IDX_EXTRAS, 10 | BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, F2}, {BOARD_IDX_WHITE + PAWN, F2}},
    {ComposeMove(tzcnt(F2), tzcnt(F4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)},
    {{BOARD_IDX_EXTRAS, (0 | BOARD_MASK_BLACK_TURN) | (tzcnt(F3) << BOARD_SHIFT_EN_PASSANT)},
     {BOARD_IDX_WHITE, F4},
     {BOARD_IDX_WHITE + PAWN, F4}},
};
const MakeUnmakeMoveTestParameter kEnPassantCapture{
    {{BOARD_IDX_EXTRAS, 11 | BOARD_MASK_WHITE_TURN | (tzcnt(G7) << BOARD_SHIFT_EN_PASSANT)},
     {BOARD_IDX_WHITE, H6},
     {BOARD_IDX_WHITE + PAWN, H6},
     {BOARD_IDX_BLACK, G6},
     {BOARD_IDX_BLACK + PAWN, G6}},
    {ComposeMove(tzcnt(H6), tzcnt(G7), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_WHITE, G7},
     {BOARD_IDX_WHITE + PAWN, G7},
     {BOARD_IDX_BLACK, XX},
     {BOARD_IDX_BLACK + PAWN, XX}},
};
const MakeUnmakeMoveTestParameter kKingSideCastling{
    {{BOARD_IDX_EXTRAS, 12 | BOARD_MASK_WHITE_TURN | BOARD_VALUE_CASTLING_WHITE_KINGSIDE},
     {BOARD_IDX_WHITE, E1 | H1},
     {BOARD_IDX_WHITE + KING, E1},
     {BOARD_IDX_WHITE + ROOK, H1}},
    {ComposeMove(tzcnt(E1), tzcnt(G1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_KINGSIDE_CASTLING)},
    {{BOARD_IDX_EXTRAS, 13 | BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_WHITE, F1 | G1},
     {BOARD_IDX_WHITE + KING, G1},
     {BOARD_IDX_WHITE + ROOK, F1}},
};
const MakeUnmakeMoveTestParameter kQueenSideCastling{
    {{BOARD_IDX_EXTRAS, 13 | BOARD_MASK_BLACK_TURN | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE},
     {BOARD_IDX_BLACK, E8 | A8},
     {BOARD_IDX_BLACK + KING, E8},
     {BOARD_IDX_BLACK + ROOK, A8}},
    {ComposeMove(tzcnt(E8), tzcnt(C8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUEENSIDE_CASTLING)},
    {{BOARD_IDX_EXTRAS, 14 | BOARD_MASK_WHITE_TURN},
     {BOARD_IDX_BLACK, C8 | D8},
     {BOARD_IDX_BLACK + KING, C8},
     {BOARD_IDX_BLACK + ROOK, D8}},
};
const MakeUnmakeMoveTestParameter kPromotion{
    {{BOARD_IDX_EXTRAS, 14 | BOARD_MASK_BLACK_TURN}, {BOARD_IDX_BLACK, H2}, {BOARD_IDX_BLACK + PAWN, H2}},
    {ComposeMove(tzcnt(H2), tzcnt(H1), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_WHITE_TURN},
     {BOARD_IDX_BLACK, H1},
     {BOARD_IDX_BLACK + PAWN, XX},
     {BOARD_IDX_BLACK + QUEEN, H1}},
};
const MakeUnmakeMoveTestParameter kPromotionCapture{
    {{BOARD_IDX_EXTRAS, 15 | BOARD_MASK_WHITE_TURN},
     {BOARD_IDX_BLACK, C8},
     {BOARD_IDX_BLACK + KNIGHT, C8},
     {BOARD_IDX_WHITE, B7},
     {BOARD_IDX_WHITE + PAWN, B7}},
    {ComposeMove(tzcnt(B7), tzcnt(C8), PAWN, KNIGHT, ROOK, MOVE_VALUE_TYPE_PROMOTION)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_BLACK, XX},
     {BOARD_IDX_BLACK + KNIGHT, XX},
     {BOARD_IDX_WHITE, C8},
     {BOARD_IDX_WHITE + PAWN, XX},
     {BOARD_IDX_WHITE + ROOK, C8}},
};

INSTANTIATE_TEST_SUITE_P(AllMoves,
                         MakeUnmakeMoveTestFixture,
                         ::testing::ValuesIn({
                             kSideSwitchedFromWhiteToBlack,
                             kSideSwitchedFromBlackToWhite,
                             kEnPassantCleared,
                             kBlackPieceMoved,
                             kWhitePieceMoved,
                             kQuietNonPawn,
                             kCapture,
                             kPawnSinglePush,
                             kPawnDoublePush,
                             kEnPassantCapture,
                             kKingSideCastling,
                             kQueenSideCastling,
                             kPromotion,
                         }));

// TODO: Add tests for revoking castling rights on:
// king rook move
// queen rook move
// king rook capture
// queen rook capture
// king move
// king capture

}  // namespace
}  // namespace Chess
