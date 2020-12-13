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
    unit.boards_[BOARD_IDX_WHITE] = 9;
    unit.boards_[BOARD_IDX_WHITE + PAWN] = 10;
    unit.boards_[BOARD_IDX_WHITE + KNIGHT] = 11;
    unit.boards_[BOARD_IDX_WHITE + BISHOP] = 12;
    unit.boards_[BOARD_IDX_WHITE + ROOK] = 13;
    unit.boards_[BOARD_IDX_WHITE + QUEEN] = 14;
    unit.boards_[BOARD_IDX_WHITE + KING] = 15;

    Bitboard expected_unique_number = 1;
    for (Bitboard board : unit.boards_)
    {
        EXPECT_EQ(board, expected_unique_number);
        expected_unique_number++;
    }
}

TEST(MakeUnmakeMoveTest, GivenMove_ExpectHistoryIsAppended)
{
    // Setup
    const Bitmove arbitrary_move{};
    const Bitboard arbitrary_extras = 42;
    Position position{};
    position.boards_[BOARD_IDX_EXTRAS] = arbitrary_extras;

    // Call
    position.MakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.extras_history_.at(0), arbitrary_extras);
    EXPECT_THAT(std::distance(position.extras_history_.begin(), position.extras_history_insertion_index_), 1);

    // Call
    position.UnmakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(std::distance(position.extras_history_.begin(), position.extras_history_insertion_index_), 0);
}

TEST(MakeUnmakeMoveTest, GivenWhiteMove_ExpectBlackAfterwards)
{
    // Setup
    constexpr Bitmove arbitrary_move{};
    Position position{};
    position.white_to_move_ = true;

    // Call
    position.MakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.white_to_move_, testing::IsFalse());

    // Call
    position.UnmakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.white_to_move_, testing::IsTrue());
}

TEST(MakeUnmakeMoveTest, GivenBlackMove_ExpectWhiteAfterwards)
{
    // Setup
    constexpr Bitmove arbitrary_move{};
    Position position{};
    position.white_to_move_ = false;

    // Call
    position.MakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.white_to_move_, testing::IsTrue());

    // Call
    position.UnmakeMove(arbitrary_move);

    // Expect
    EXPECT_THAT(position.white_to_move_, testing::IsFalse());
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
    bool prior_white_to_move;
    Bitmove move;
    std::vector<PosteriorSpecifier> posterior_setup;
    std::string description;
};

class MakeUnmakeMoveTestFixture : public ::testing::TestWithParam<MakeUnmakeMoveTestParameter>
{
};

TEST_P(MakeUnmakeMoveTestFixture, GivenMove_ExpectPosteriorAfterMakeAndPriorAfterUnmake)
{
    // Setup
    Position position{};
    position.white_to_move_ = GetParam().prior_white_to_move;
    for (const auto& setter : GetParam().prior_setup)
    {
        position[setter.index] = setter.value;
    }

    std::array<::testing::Matcher<Bitboard>, 15> expected_position_posterior{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    for (const auto& setter : GetParam().posterior_setup)
    {
        expected_position_posterior[setter.index] = setter.value;
    }

    const Position expected_position_prior{position};

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

MATCHER(HasEnPassantSet, "")
{
    return arg & BOARD_MASK_EN_PASSANT;
}

MATCHER(HasCastlingOnLastMoveSet, "")
{
    return arg & (BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE | BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE);
}

const MakeUnmakeMoveTestParameter kEnPassantCleared{
    {{BOARD_IDX_EXTRAS, BOARD_MASK_EN_PASSANT}},
    true,
    {},
    {{BOARD_IDX_EXTRAS, ::testing::Not(HasEnPassantSet())}},
    "EnPassantCleared",
};
const MakeUnmakeMoveTestParameter kCastlingOnLastMoveCleared{
    {{BOARD_IDX_EXTRAS, BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE | BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE}},
    true,
    {},
    {{BOARD_IDX_EXTRAS, ::testing::Not(HasCastlingOnLastMoveSet())}},
    "CastlingOnLastMoveCleared",
};
const MakeUnmakeMoveTestParameter kBlackPieceMoved{
    {{BOARD_IDX_BLACK, A7}},
    false,
    {ComposeMove(tzcnt(A7), tzcnt(A6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 1}, {BOARD_IDX_BLACK, A6}, {BOARD_IDX_BLACK + ROOK, IsAnything()}},
    "BlackPieceMoved",
};
const MakeUnmakeMoveTestParameter kWhitePieceMoved{
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_WHITE, A7}},
    true,
    {ComposeMove(tzcnt(A7), tzcnt(A6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 1}, {BOARD_IDX_WHITE, A6}, {BOARD_IDX_WHITE + ROOK, IsAnything()}},
    "WhitePieceMoved",
};
const MakeUnmakeMoveTestParameter kQuietNonPawn{
    {{BOARD_IDX_EXTRAS, 2}, {BOARD_IDX_WHITE, H1}, {BOARD_IDX_WHITE + KNIGHT, H1}},
    true,
    {ComposeMove(tzcnt(H1), tzcnt(G3), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 3}, {BOARD_IDX_WHITE, G3}, {BOARD_IDX_WHITE + KNIGHT, G3}},
    "QuietNonPawn",
};
const MakeUnmakeMoveTestParameter kCapture{
    {{BOARD_IDX_EXTRAS, 3},
     {BOARD_IDX_WHITE, D5},
     {BOARD_IDX_WHITE + PAWN, D5},
     {BOARD_IDX_BLACK, D1},
     {BOARD_IDX_BLACK + QUEEN, D1}},
    false,
    {ComposeMove(tzcnt(D1), tzcnt(D5), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0},
     {BOARD_IDX_WHITE, XX},
     {BOARD_IDX_WHITE + PAWN, XX},
     {BOARD_IDX_BLACK, D5},
     {BOARD_IDX_BLACK + QUEEN, D5}},
    "Capture",
};
const MakeUnmakeMoveTestParameter kPawnSinglePush{
    {{BOARD_IDX_EXTRAS, 9}, {BOARD_IDX_BLACK, F3}, {BOARD_IDX_BLACK + PAWN, F3}},
    false,
    {ComposeMove(tzcnt(F3), tzcnt(F2), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_BLACK, F2}, {BOARD_IDX_BLACK + PAWN, F2}},
    "PawnSinglePush",
};
const MakeUnmakeMoveTestParameter kPawnDoublePush{
    {{BOARD_IDX_EXTRAS, 10}, {BOARD_IDX_WHITE, F2}, {BOARD_IDX_WHITE + PAWN, F2}},
    true,
    {ComposeMove(tzcnt(F2), tzcnt(F4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)},
    {{BOARD_IDX_EXTRAS, (tzcnt(F3) << BOARD_SHIFT_EN_PASSANT)}, {BOARD_IDX_WHITE, F4}, {BOARD_IDX_WHITE + PAWN, F4}},
    "PawnDoublePush",
};
const MakeUnmakeMoveTestParameter kEnPassantCapture{
    {{BOARD_IDX_EXTRAS, 11 | (tzcnt(G7) << BOARD_SHIFT_EN_PASSANT)},
     {BOARD_IDX_WHITE, H6},
     {BOARD_IDX_WHITE + PAWN, H6},
     {BOARD_IDX_BLACK, G6},
     {BOARD_IDX_BLACK + PAWN, G6}},
    true,
    {ComposeMove(tzcnt(H6), tzcnt(G7), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0},
     {BOARD_IDX_WHITE, G7},
     {BOARD_IDX_WHITE + PAWN, G7},
     {BOARD_IDX_BLACK, XX},
     {BOARD_IDX_BLACK + PAWN, XX}},
    "EnPassantCapture",
};
const MakeUnmakeMoveTestParameter kKingSideCastling{
    {{BOARD_IDX_EXTRAS, 12 | BOARD_VALUE_CASTLING_WHITE_KINGSIDE},
     {BOARD_IDX_WHITE, E1 | H1},
     {BOARD_IDX_WHITE + KING, E1},
     {BOARD_IDX_WHITE + ROOK, H1}},
    true,
    {ComposeMove(tzcnt(E1), tzcnt(G1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_KINGSIDE_CASTLING)},
    {{BOARD_IDX_EXTRAS, 13 | BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE},
     {BOARD_IDX_WHITE, F1 | G1},
     {BOARD_IDX_WHITE + KING, G1},
     {BOARD_IDX_WHITE + ROOK, F1}},
    "KingSideCastling",
};
const MakeUnmakeMoveTestParameter kQueenSideCastling{
    {{BOARD_IDX_EXTRAS, 13 | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE},
     {BOARD_IDX_BLACK, E8 | A8},
     {BOARD_IDX_BLACK + KING, E8},
     {BOARD_IDX_BLACK + ROOK, A8}},
    false,
    {ComposeMove(tzcnt(E8), tzcnt(C8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUEENSIDE_CASTLING)},
    {{BOARD_IDX_EXTRAS, 14 | BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE},
     {BOARD_IDX_BLACK, C8 | D8},
     {BOARD_IDX_BLACK + KING, C8},
     {BOARD_IDX_BLACK + ROOK, D8}},
    "QueenSideCastling",
};
const MakeUnmakeMoveTestParameter kPromotion{
    {{BOARD_IDX_EXTRAS, 14}, {BOARD_IDX_BLACK, H2}, {BOARD_IDX_BLACK + PAWN, H2}},
    false,
    {ComposeMove(tzcnt(H2), tzcnt(H1), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_BLACK, H1}, {BOARD_IDX_BLACK + PAWN, XX}, {BOARD_IDX_BLACK + QUEEN, H1}},
    "Promotion",
};
const MakeUnmakeMoveTestParameter kPromotionCapture{
    {{BOARD_IDX_EXTRAS, 15},
     {BOARD_IDX_BLACK, C8},
     {BOARD_IDX_BLACK + KNIGHT, C8},
     {BOARD_IDX_WHITE, B7},
     {BOARD_IDX_WHITE + PAWN, B7}},
    false,
    {ComposeMove(tzcnt(B7), tzcnt(C8), PAWN, KNIGHT, ROOK, MOVE_VALUE_TYPE_PROMOTION)},
    {{BOARD_IDX_EXTRAS, 0},
     {BOARD_IDX_BLACK, XX},
     {BOARD_IDX_BLACK + KNIGHT, XX},
     {BOARD_IDX_WHITE, C8},
     {BOARD_IDX_WHITE + PAWN, XX},
     {BOARD_IDX_WHITE + ROOK, C8}},
    "PromotionCapture",
};
const MakeUnmakeMoveTestParameter kKingMoveLosesCastling{
    {{BOARD_IDX_EXTRAS, 16 | BOARD_VALUE_CASTLING_WHITE_KINGSIDE | BOARD_VALUE_CASTLING_WHITE_QUEENSIDE},
     {BOARD_IDX_WHITE, E1},
     {BOARD_IDX_WHITE + KING, E1}},
    true,
    {ComposeMove(tzcnt(E1), tzcnt(E2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 17}, {BOARD_IDX_WHITE, E2}, {BOARD_IDX_WHITE + KING, E2}},
    "KingMoveLosesCastling",
};
const MakeUnmakeMoveTestParameter kWhiteKingRookMoveLosesCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_VALUE_CASTLING_WHITE_KINGSIDE}, {BOARD_IDX_WHITE, H1}, {BOARD_IDX_WHITE + ROOK, H1}},
    true,
    {ComposeMove(tzcnt(H1), tzcnt(H2), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 18}, {BOARD_IDX_WHITE, H2}, {BOARD_IDX_WHITE + ROOK, H2}},
    "KingRookMoveLosesCastling",
};
const MakeUnmakeMoveTestParameter kWhiteQueenRookMoveLosesCastling{
    {{BOARD_IDX_EXTRAS, 18 | BOARD_VALUE_CASTLING_WHITE_QUEENSIDE},
     {BOARD_IDX_WHITE, A1},
     {BOARD_IDX_WHITE + ROOK, A1}},
    true,
    {ComposeMove(tzcnt(A1), tzcnt(B1), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 19}, {BOARD_IDX_WHITE, B1}, {BOARD_IDX_WHITE + ROOK, B1}},
    "WhiteQueenRookMoveLosesCastling",
};
const MakeUnmakeMoveTestParameter kBlackKingRookMoveLosesCastling{
    {{BOARD_IDX_EXTRAS, 20 | BOARD_VALUE_CASTLING_BLACK_KINGSIDE}, {BOARD_IDX_BLACK, H8}, {BOARD_IDX_BLACK + ROOK, H8}},
    false,
    {ComposeMove(tzcnt(H8), tzcnt(F8), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 21}, {BOARD_IDX_BLACK, F8}, {BOARD_IDX_BLACK + ROOK, F8}},
    "BlackKingRookMoveLosesCastling",
};
const MakeUnmakeMoveTestParameter kBlackQueenRookMoveLosesCastling{
    {{BOARD_IDX_EXTRAS, 21 | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE},
     {BOARD_IDX_BLACK, A8},
     {BOARD_IDX_BLACK + ROOK, A8}},
    false,
    {ComposeMove(tzcnt(A8), tzcnt(A3), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)},
    {{BOARD_IDX_EXTRAS, 22}, {BOARD_IDX_BLACK, A3}, {BOARD_IDX_BLACK + ROOK, A3}},
    "BlackQueenRookMoveLosesCastling",
};
const MakeUnmakeMoveTestParameter kWhiteKingRookCapturedLosesCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_VALUE_CASTLING_WHITE_KINGSIDE},
     {BOARD_IDX_WHITE, H1},
     {BOARD_IDX_WHITE + ROOK, H1},
     {BOARD_IDX_BLACK, H8},
     {BOARD_IDX_BLACK + QUEEN, H8}},
    false,
    {ComposeMove(tzcnt(H8), tzcnt(H1), QUEEN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_BLACK, H1}, {BOARD_IDX_BLACK + QUEEN, H1}},
    "KingRookCapturedLosesCastling",
};
const MakeUnmakeMoveTestParameter kWhiteQueenRookCapturedLosesCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_VALUE_CASTLING_WHITE_QUEENSIDE},
     {BOARD_IDX_WHITE, A1},
     {BOARD_IDX_WHITE + ROOK, A1},
     {BOARD_IDX_BLACK, A8},
     {BOARD_IDX_BLACK + QUEEN, A8}},
    false,
    {ComposeMove(tzcnt(A8), tzcnt(A1), QUEEN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_BLACK, A1}, {BOARD_IDX_BLACK + QUEEN, A1}},
    "WhiteQueenRookCapturedLosesCastling",
};
const MakeUnmakeMoveTestParameter kBlackKingRookCapturedLosesCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_VALUE_CASTLING_BLACK_KINGSIDE},
     {BOARD_IDX_BLACK, H8},
     {BOARD_IDX_BLACK + ROOK, H8},
     {BOARD_IDX_WHITE, H1},
     {BOARD_IDX_WHITE + QUEEN, H1}},
    true,
    {ComposeMove(tzcnt(H1), tzcnt(H8), QUEEN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_WHITE, H8}, {BOARD_IDX_WHITE + QUEEN, H8}},
    "BlackKingRookCapturedLosesCastling",
};
const MakeUnmakeMoveTestParameter kBlackQueenRookCapturedLosesCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE},
     {BOARD_IDX_BLACK, A8},
     {BOARD_IDX_BLACK + ROOK, A8},
     {BOARD_IDX_WHITE, A1},
     {BOARD_IDX_WHITE + QUEEN, A1}},
    true,
    {ComposeMove(tzcnt(A1), tzcnt(A8), QUEEN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)},
    {{BOARD_IDX_EXTRAS, 0}, {BOARD_IDX_WHITE, A8}, {BOARD_IDX_WHITE + QUEEN, A8}},
    "BlackQueenRookCapturedLosesCastling",
};
const MakeUnmakeMoveTestParameter kQuietMoveDoesntAffectCastling{
    {{BOARD_IDX_EXTRAS, 17 | BOARD_MASK_CASTLING},
     {BOARD_IDX_BLACK, A8 | E8 | H8 | F5},
     {BOARD_IDX_BLACK + ROOK, A8 | H8},
     {BOARD_IDX_BLACK + KING, E8},
     {BOARD_IDX_BLACK + PAWN, F5},
     {BOARD_IDX_WHITE, A1 | E1 | H1},
     {BOARD_IDX_WHITE + ROOK, A1 | H1},
     {BOARD_IDX_WHITE + KING, E1}},
    false,
    {ComposeMove(tzcnt(F5), tzcnt(F4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)},
    {{BOARD_IDX_EXTRAS, 0 | BOARD_MASK_CASTLING},
     {BOARD_IDX_BLACK, A8 | E8 | H8 | F4},
     {BOARD_IDX_BLACK + ROOK, A8 | H8},
     {BOARD_IDX_BLACK + KING, E8},
     {BOARD_IDX_BLACK + PAWN, F4},
     {BOARD_IDX_WHITE, A1 | E1 | H1},
     {BOARD_IDX_WHITE + ROOK, A1 | H1},
     {BOARD_IDX_WHITE + KING, E1}},
    "QuietMoveDoesntAffectCastling",
};

INSTANTIATE_TEST_SUITE_P(AllMoves,
                         MakeUnmakeMoveTestFixture,
                         ::testing::Values(kEnPassantCleared,
                                           kCastlingOnLastMoveCleared,
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
                                           kKingMoveLosesCastling,
                                           kWhiteKingRookMoveLosesCastling,
                                           kWhiteQueenRookMoveLosesCastling,
                                           kBlackKingRookMoveLosesCastling,
                                           kBlackQueenRookMoveLosesCastling,
                                           kWhiteKingRookCapturedLosesCastling,
                                           kWhiteQueenRookCapturedLosesCastling,
                                           kBlackKingRookCapturedLosesCastling,
                                           kBlackQueenRookCapturedLosesCastling,
                                           kQuietMoveDoesntAffectCastling),
                         [](const auto& info) { return info.param.description; });

struct DefendersKingIsInCheckTestParameter
{
    Bitboard extras;
    bool white_to_move;
    std::vector<std::pair<std::size_t, Bitboard>> white_pieces;
    std::vector<std::pair<std::size_t, Bitboard>> black_pieces;
    bool expected_king_safety;
    std::string description;
};

class DefendersKingIsInCheckTestFixture : public ::testing::TestWithParam<DefendersKingIsInCheckTestParameter>
{
};

TEST_P(DefendersKingIsInCheckTestFixture, GivenPosition_ExpectProvidedStateOfKingSafety)
{
    Position position{};
    position.white_to_move_ = GetParam().white_to_move;
    position[BOARD_IDX_EXTRAS] |= GetParam().extras;
    for (const auto& piece : GetParam().white_pieces)
    {
        position[BOARD_IDX_WHITE] |= piece.second;
        position[BOARD_IDX_WHITE + piece.first] |= piece.second;
    }
    for (const auto& piece : GetParam().black_pieces)
    {
        position[BOARD_IDX_BLACK] |= piece.second;
        position[BOARD_IDX_BLACK + piece.first] |= piece.second;
    }

    EXPECT_THAT(position.DefendersKingIsInCheck(), GetParam().expected_king_safety);
}

const DefendersKingIsInCheckTestParameter kByBlackRook{{}, false, {{KING, A1}}, {{ROOK, A8}}, true, "ByBlackRook"};
const DefendersKingIsInCheckTestParameter kNotByBlackRook{{},
                                                          false,
                                                          {{KING, A1}},
                                                          {{ROOK, B8}},
                                                          false,
                                                          "NotByBlackRook"};
const DefendersKingIsInCheckTestParameter kByWhiteRook{{}, true, {{ROOK, B6}}, {{KING, E6}}, true, "ByWhiteRook"};
const DefendersKingIsInCheckTestParameter kNotByWhiteRook{{},
                                                          true,
                                                          {{ROOK, B7}},
                                                          {{KING, E6}},
                                                          false,
                                                          "NotByWhiteRook"};
const DefendersKingIsInCheckTestParameter kByBlackBishop{{},
                                                         false,
                                                         {{KING, H8}},
                                                         {{BISHOP, B2}},
                                                         true,
                                                         "ByBlackBishop"};
const DefendersKingIsInCheckTestParameter kNotByBlackBishop{{},
                                                            true,
                                                            {{KING, H8}},
                                                            {{BISHOP, C2}},
                                                            false,
                                                            "NotByBlackBishop"};
const DefendersKingIsInCheckTestParameter kByWhiteBishop{{}, true, {{BISHOP, A7}}, {{KING, G1}}, true, "ByWhiteBishop"};
const DefendersKingIsInCheckTestParameter kNotByWhiteBishop{{},
                                                            true,
                                                            {{BISHOP, A8}},
                                                            {{KING, G1}},
                                                            false,
                                                            "NotByWhiteBishop"};
const DefendersKingIsInCheckTestParameter kPinnedPawn{{},
                                                      true,
                                                      {{BISHOP, A7}},
                                                      {{KING, G1}, {PAWN, F2}},
                                                      false,
                                                      "PinnedPawn"};
const DefendersKingIsInCheckTestParameter kPawnObstructing{{},
                                                           true,
                                                           {{BISHOP, A7}, {PAWN, F2}},
                                                           {{KING, G1}},
                                                           false,
                                                           "PawnObstructing"};
const DefendersKingIsInCheckTestParameter kByBlackKnight{{},
                                                         false,
                                                         {{KING, F4}},
                                                         {{KNIGHT, H5}},
                                                         true,
                                                         "ByBlackKnight"};
const DefendersKingIsInCheckTestParameter kNotByBlackKnight{{},
                                                            false,
                                                            {{KING, F5}},
                                                            {{KNIGHT, H5}},
                                                            false,
                                                            "NotByBlackKnight"};
const DefendersKingIsInCheckTestParameter kByWhiteKnight{{}, true, {{KNIGHT, C3}}, {{KING, D5}}, true, "ByWhiteKnight"};
const DefendersKingIsInCheckTestParameter kNotByWhiteKnight{{},
                                                            true,
                                                            {{KNIGHT, C3}},
                                                            {{KING, D6}},
                                                            false,
                                                            "NotByWhiteKnight"};
const DefendersKingIsInCheckTestParameter kByWhitePawn1{{}, true, {{PAWN, C3}}, {{KING, D4}}, true, "ByWhitePawn1"};
const DefendersKingIsInCheckTestParameter kByWhitePawn2{{}, true, {{PAWN, E3}}, {{KING, D4}}, true, "ByWhitePawn2"};
const DefendersKingIsInCheckTestParameter kNotByWhitePawn{{},
                                                          true,
                                                          {{PAWN, D3}},
                                                          {{KING, D4}},
                                                          false,
                                                          "NotByWhitePawn"};
const DefendersKingIsInCheckTestParameter kByBlackPawn1{{}, false, {{KING, D4}}, {{PAWN, C5}}, true, "ByBlackPawn1"};
const DefendersKingIsInCheckTestParameter kByBlackPawn2{{}, false, {{KING, D4}}, {{PAWN, E5}}, true, "ByBlackPawn2"};
const DefendersKingIsInCheckTestParameter kNotByBlackPawn{{},
                                                          false,
                                                          {{KING, D4}},
                                                          {{PAWN, D5}},
                                                          false,
                                                          "NotByBlackPawn"};
const DefendersKingIsInCheckTestParameter kWhiteAfterCastlingKingsidePassThrough{
    BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE,
    false,
    {{KING, G1}, {ROOK, F1}},
    {{ROOK, F8}},
    true,
    "WhiteAfterCastlingKingsidePassThrough"};
const DefendersKingIsInCheckTestParameter kWhiteAfterCastlingKingsideOriginalSquare{
    BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE,
    false,
    {{KING, G1}, {ROOK, F1}},
    {{ROOK, E8}},
    true,
    "WhiteAfterCastlingKingsideOriginalSquare"};
const DefendersKingIsInCheckTestParameter kWhiteAfterCastlingQueensidePassThrough{
    BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE,
    false,
    {{KING, C1}, {ROOK, D1}},
    {{ROOK, D8}},
    true,
    "WhiteAfterCastlingQueensidePassThrough"};
const DefendersKingIsInCheckTestParameter kWhiteAfterCastlingQueensideOriginalSquare{
    BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE,
    false,
    {{KING, C1}, {ROOK, D1}},
    {{ROOK, E8}},
    true,
    "WhiteAfterCastlingQueensideOriginalSquare"};
const DefendersKingIsInCheckTestParameter kBlackAfterCastlingKingsidePassThrough{
    BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE,
    true,
    {{ROOK, F1}},
    {{KING, G8}, {ROOK, F8}},
    true,
    "BlackAfterCastlingKingsidePassThrough"};
const DefendersKingIsInCheckTestParameter kBlackAfterCastlingKingsideOriginalSquare{
    BOARD_VALUE_KINGSIDE_CASTLING_ON_LAST_MOVE,
    true,
    {{ROOK, E1}},
    {{KING, G8}, {ROOK, F8}},
    true,
    "BlackAfterCastlingKingsideOriginalSquare"};
const DefendersKingIsInCheckTestParameter kBlackAfterCastlingQueensidePassThrough{
    BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE,
    true,
    {{ROOK, D1}},
    {{KING, C8}, {ROOK, D8}},
    true,
    "BlackAfterCastlingQueensidePassThrough"};
const DefendersKingIsInCheckTestParameter kBlackAfterCastlingQueensideOriginalSquare{
    BOARD_VALUE_QUEENSIDE_CASTLING_ON_LAST_MOVE,
    true,
    {{ROOK, E1}},
    {{KING, C8}, {ROOK, D8}},
    true,
    "BlackAfterCastlingQueensideOriginalSquare"};
const DefendersKingIsInCheckTestParameter kBlackKingByWhiteKing{{},
                                                                true,
                                                                {{KING, E1}},
                                                                {{KING, F1}},
                                                                true,
                                                                "BlackKingByWhiteKing"};
const DefendersKingIsInCheckTestParameter kWhiteKingByBlackKing{{},
                                                                false,
                                                                {{KING, E1}},
                                                                {{KING, F1}},
                                                                true,
                                                                "WhiteKingByBlackKing"};

INSTANTIATE_TEST_SUITE_P(AllAttackingAngles,
                         DefendersKingIsInCheckTestFixture,
                         ::testing::Values(kByBlackRook,
                                           kNotByBlackRook,
                                           kByWhiteRook,
                                           kNotByWhiteRook,
                                           kByBlackBishop,
                                           kNotByBlackBishop,
                                           kByWhiteBishop,
                                           kNotByWhiteBishop,
                                           kByBlackKnight,
                                           kNotByBlackKnight,
                                           kByWhiteKnight,
                                           kNotByWhiteKnight,
                                           kByWhitePawn1,
                                           kByWhitePawn2,
                                           kNotByWhitePawn,
                                           kByBlackPawn1,
                                           kByBlackPawn2,
                                           kNotByBlackPawn,
                                           kPinnedPawn,
                                           kPawnObstructing,
                                           kWhiteAfterCastlingKingsidePassThrough,
                                           kWhiteAfterCastlingKingsideOriginalSquare,
                                           kWhiteAfterCastlingQueensidePassThrough,
                                           kWhiteAfterCastlingQueensideOriginalSquare,
                                           kBlackAfterCastlingKingsidePassThrough,
                                           kBlackAfterCastlingKingsideOriginalSquare,
                                           kBlackAfterCastlingQueensidePassThrough,
                                           kBlackAfterCastlingQueensideOriginalSquare,
                                           kBlackKingByWhiteKing,
                                           kWhiteKingByBlackKing),
                         [](const auto& info) { return info.param.description; });

}  // namespace
}  // namespace Chess
