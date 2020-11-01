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

bool BoardsEqualTheirIndexExceptIgnored(const PositionWithBitboards& position,
                                        const std::vector<std::size_t> indexes_to_ignore)
{
    bool return_value = true;
    for (std::size_t index{0}; index < position.boards_.size(); index++)
    {
        if (!std::any_of(
                begin(indexes_to_ignore), end(indexes_to_ignore), [&index](auto elem) { return elem == index; }))
        {
            if (position[index] != index)
            {
                return_value = false;
            }
        }
    }
    return return_value;
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
    {{BOARD_IDX_EXTRAS, 1 + BOARD_MASK_WHITE_TURN}, {BOARD_IDX_BLACK, A6}, {BOARD_IDX_BLACK + ROOK, IsAnything()}},
};
const MakeUnmakeMoveTestParameter kWhitePieceMoved{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, A7}},
    {ComposeMove(tzcnt(A7), tzcnt(A6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 1 + BOARD_MASK_BLACK_TURN}, {BOARD_IDX_WHITE, A6}, {BOARD_IDX_WHITE + ROOK, IsAnything()}},
};
const MakeUnmakeMoveTestParameter kQuietNonPawn{
    {{BOARD_IDX_EXTRAS, 2 + BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, H1}, {BOARD_IDX_WHITE + KNIGHT, H1}},
    {ComposeMove(tzcnt(H1), tzcnt(G3), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 3 + BOARD_MASK_BLACK_TURN}, {BOARD_IDX_WHITE, G3}, {BOARD_IDX_WHITE + KNIGHT, G3}},
};
const MakeUnmakeMoveTestParameter kCapture{
    {{BOARD_IDX_EXTRAS, 3 + BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_WHITE, D5},
     {BOARD_IDX_WHITE + PAWN, D5},
     {BOARD_IDX_BLACK, D1},
     {BOARD_IDX_BLACK + QUEEN, D1}},
    {ComposeMove(tzcnt(D1), tzcnt(D5), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0 + BOARD_MASK_WHITE_TURN},
     {BOARD_IDX_WHITE, XX},
     {BOARD_IDX_WHITE + PAWN, XX},
     {BOARD_IDX_BLACK, D5},
     {BOARD_IDX_BLACK + QUEEN, D5}},
};
const MakeUnmakeMoveTestParameter kPawnSinglePush{
    {{BOARD_IDX_EXTRAS, 9 + BOARD_MASK_BLACK_TURN}, {BOARD_IDX_BLACK, F3}, {BOARD_IDX_BLACK + PAWN, F3}},
    {ComposeMove(tzcnt(F3), tzcnt(F2), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)},
    {{BOARD_IDX_EXTRAS, 0 + BOARD_MASK_WHITE_TURN}, {BOARD_IDX_BLACK, F2}, {BOARD_IDX_BLACK + PAWN, F2}},
};
const MakeUnmakeMoveTestParameter kPawnDoublePush{
    {{BOARD_IDX_EXTRAS, 10 + BOARD_MASK_WHITE_TURN}, {BOARD_IDX_WHITE, F2}, {BOARD_IDX_WHITE + PAWN, F2}},
    {ComposeMove(tzcnt(F2), tzcnt(F4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)},
    {{BOARD_IDX_EXTRAS, (0 + BOARD_MASK_BLACK_TURN) | (tzcnt(F3) << BOARD_SHIFT_EN_PASSANT)},
     {BOARD_IDX_WHITE, F4},
     {BOARD_IDX_WHITE + PAWN, F4}},
};
const MakeUnmakeMoveTestParameter kEnPassantCapture{
    {{BOARD_IDX_EXTRAS, (11 + BOARD_MASK_WHITE_TURN) | (tzcnt(G7) << BOARD_SHIFT_EN_PASSANT)},
     {BOARD_IDX_WHITE, H6},
     {BOARD_IDX_WHITE + PAWN, H6},
     {BOARD_IDX_BLACK, G6},
     {BOARD_IDX_BLACK + PAWN, G6}},
    {ComposeMove(tzcnt(H6), tzcnt(G7), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0 + BOARD_MASK_BLACK_TURN},
     {BOARD_IDX_WHITE, G7},
     {BOARD_IDX_WHITE + PAWN, G7},
     {BOARD_IDX_BLACK, XX},
     {BOARD_IDX_BLACK + PAWN, XX}},
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
                         }));

TEST(PositionMakeMoveFixture,
     GivenKingsideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked)
{
    // Setup
    const Bitboard source_king = E1;
    const Bitboard target_king = G1;
    const Bitboard source_rook = H1;
    const Bitboard target_rook = F1;
    const Bitboard static_plies = 12;
    const Bitboard extras = static_plies | BOARD_MASK_WHITE_TURN | BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
    PositionWithBitboards position{{extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, source_rook, 14, source_king, 16}};

    const Bitmove source_bit = tzcnt(source_king);
    const Bitmove target_bit = tzcnt(target_king);
    const Bitmove kingside_castling = MOVE_VALUE_TYPE_KINGSIDE_CASTLING | source_bit |
                                      (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

    const PositionWithBitboards position_prior_make_move{position};

    // Call
    position.MakeMove(kingside_castling);

    // Expect
    EXPECT_EQ(position[BOARD_IDX_WHITE + KING], target_king);
    EXPECT_EQ(position[BOARD_IDX_WHITE + ROOK], target_rook);
    EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, static_plies + 1);
    EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_WHITE_KINGSIDE);
    EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 13, 15}));

    // Call
    position.UnmakeMove(kingside_castling);

    // Expect
    EXPECT_EQ(position, position_prior_make_move);
}

TEST(PositionMakeMoveFixture,
     GivenQueensideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked)
{
    // Setup
    const Bitboard source_king = E8;
    const Bitboard target_king = C8;
    const Bitboard source_rook = A8;
    const Bitboard target_rook = D8;
    const Bitboard static_plies = 13;
    const Bitboard extras = static_plies | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
    PositionWithBitboards position{{extras, 1, 2, 3, 4, source_rook, 6, source_king, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

    const Bitmove source_bit = tzcnt(source_king);
    const Bitmove target_bit = tzcnt(target_king);
    const Bitmove queenside_castling = MOVE_VALUE_TYPE_QUEENSIDE_CASTLING | source_bit |
                                       (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

    const PositionWithBitboards position_prior_make_move{position};

    // Call
    position.MakeMove(queenside_castling);

    // Expect
    EXPECT_EQ(position[BOARD_IDX_BLACK + KING], target_king);
    EXPECT_EQ(position[BOARD_IDX_BLACK + ROOK], target_rook);
    EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, static_plies + 1);
    EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
    EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 5, 7}));

    // Call
    position.UnmakeMove(queenside_castling);

    // Expect
    EXPECT_EQ(position, position_prior_make_move);
}

TEST(PositionMakeMoveFixture, GivenPromotion_ExpectPieceMovedAndStaticPliesReset)
{
    // Setup
    const Bitboard source = H2;
    const Bitboard target = H1;
    const Bitboard static_plies = 30;
    const Bitboard extras = static_plies;
    PositionWithBitboards position{{extras, 1, source, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

    const Bitmove source_bit = tzcnt(source);
    const Bitmove target_bit = tzcnt(target);
    const Bitmove promotion_move = MOVE_VALUE_TYPE_PROMOTION | source_bit | (target_bit << MOVE_SHIFT_TARGET) |
                                   (PAWN << MOVE_SHIFT_MOVED_PIECE) | (QUEEN << MOVE_SHIFT_PROMOTION);

    const PositionWithBitboards position_prior_make_move{position};

    // Call
    position.MakeMove(promotion_move);

    // Expect
    EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], BOARD_ZEROS);
    EXPECT_EQ(position[BOARD_IDX_BLACK + QUEEN], target);
    EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
    EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2, 6}));

    // Call
    position.UnmakeMove(promotion_move);

    // Expect
    EXPECT_EQ(position, position_prior_make_move);
}

TEST(PositionMakeMoveFixture, GivenPromotionCapture_ExpectPieceMovedAndStaticPliesReset)
{
    // Setup
    const Bitboard source = B7;
    const Bitboard target = B8;
    const Bitboard static_plies = 31;
    const Bitboard extras = static_plies | BOARD_MASK_WHITE_TURN;
    PositionWithBitboards position{
        {extras, target, 2, 3, target, 5, 6, 7, 8, 9, source, 11, 12, 13, BOARD_ZEROS, 15, 16}};

    const Bitmove source_bit = tzcnt(source);
    const Bitmove target_bit = tzcnt(target);
    const Bitmove promotion_capture_move = MOVE_VALUE_TYPE_PROMOTION | source_bit | (target_bit << MOVE_SHIFT_TARGET) |
                                           (PAWN << MOVE_SHIFT_MOVED_PIECE) | (QUEEN << MOVE_SHIFT_PROMOTION) |
                                           (BISHOP << MOVE_SHIFT_CAPTURED_PIECE);

    const PositionWithBitboards position_prior_make_move{position};

    // Call
    position.MakeMove(promotion_capture_move);

    // Expect
    EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], BOARD_ZEROS);
    EXPECT_EQ(position[BOARD_IDX_WHITE + QUEEN], target);
    EXPECT_EQ(position[BOARD_IDX_BLACK + BISHOP], BOARD_ZEROS);
    EXPECT_EQ(position[BOARD_IDX_BLACK], BOARD_ZEROS);
    EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
    EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 4, 9, 10, 14}));

    // Call
    position.UnmakeMove(promotion_capture_move);

    // Expect
    EXPECT_EQ(position.boards_, position_prior_make_move.boards_);
    EXPECT_EQ(position, position_prior_make_move);
}

// TODO: Add missing test for history was appended

// TODO: Add tests for revoking castling rights on:
// king rook move
// queen rook move
// king rook capture
// queen rook capture
// king move
// king capture

}  // namespace
}  // namespace Chess
