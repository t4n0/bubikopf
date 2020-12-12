#include "bitboard/move_generation.h"
#include "bitboard/squares.h"
#include "hardware/population_count.h"
#include "hardware/trailing_zeros_count.h"

#include <gmock/gmock.h>

namespace Chess
{
namespace
{

struct MoveGenerationTestParameter
{
    Bitboard extras;
    bool white_to_move;
    std::vector<std::pair<std::size_t, Bitboard>> white_pieces;
    std::vector<std::pair<std::size_t, Bitboard>> black_pieces;
    std::vector<Bitmove> expected_moves;
};

class MoveGenerationTestFixture : public ::testing::TestWithParam<MoveGenerationTestParameter>
{
  public:
    void SetUpBoardAccordingToTestParameter()
    {
        position_[BOARD_IDX_EXTRAS] |= GetParam().extras;
        position_.white_to_move_ = GetParam().white_to_move;

        for (const auto& piece : GetParam().black_pieces)
        {
            position_[BOARD_IDX_BLACK] |= piece.second;
            position_[BOARD_IDX_BLACK + piece.first] |= piece.second;
        }
        for (const auto& piece : GetParam().white_pieces)
        {
            position_[BOARD_IDX_WHITE] |= piece.second;
            position_[BOARD_IDX_WHITE + piece.first] |= piece.second;
        }
    }

    Position position_{};
    MoveList move_list_{};
};

TEST_P(MoveGenerationTestFixture, GivenAtomicPosition_ExpectAllPseudoLegalMoves)
{
    SetUpBoardAccordingToTestParameter();

    const MoveList::iterator returned_move_insertion_iterator = GenerateMoves(position_, move_list_.begin());

    const std::size_t returned_number_of_moves = std::distance(move_list_.begin(), returned_move_insertion_iterator);
    const std::size_t expected_number_of_moves = GetParam().expected_moves.size();
    ASSERT_EQ(returned_number_of_moves, expected_number_of_moves) << ToString(move_list_);

    const std::vector<Bitmove> returned_moves(move_list_.begin(), returned_move_insertion_iterator);
    const std::vector<Bitmove>& expected_moves = GetParam().expected_moves;
    EXPECT_THAT(returned_moves, ::testing::UnorderedElementsAreArray(expected_moves)) << ToString(move_list_);
}

// white pawn moves
const MoveGenerationTestParameter kWhitePawnSinglePush{
    {},
    true,
    {{PAWN, E5}},
    {},
    {ComposeMove(tzcnt(E5), tzcnt(E6), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)}};
const MoveGenerationTestParameter kWhitePawnBlockedMidfield{{}, true, {{PAWN, D5}}, {{PAWN, D6}}, {}};
const MoveGenerationTestParameter kWhitePawnOnStartRow{
    {},
    true,
    {{PAWN, A2}},
    {},
    {ComposeMove(tzcnt(A2), tzcnt(A3), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH),
     ComposeMove(tzcnt(A2), tzcnt(A4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)}};
const MoveGenerationTestParameter kWhitePawnBlockedOnStartRow{{}, true, {{PAWN, C2}}, {{PAWN, C3}}, {}};
const MoveGenerationTestParameter kWhitePawnCaptures{
    {},
    true,
    {{PAWN, C5}},
    {{PAWN, C6}, {KNIGHT, B6}, {ROOK, D6}},
    {ComposeMove(tzcnt(C5), tzcnt(B6), PAWN, KNIGHT, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(C5), tzcnt(D6), PAWN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnEnPassantOptionRight{
    (static_cast<Bitboard>(tzcnt(F6)) << BOARD_SHIFT_EN_PASSANT),
    true,
    {{PAWN, E5}},
    {{PAWN, E6}, {PAWN, F5}},
    {ComposeMove(tzcnt(E5), tzcnt(F6), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnEnPassantOptionLeft{
    (static_cast<Bitboard>(tzcnt(D6)) << BOARD_SHIFT_EN_PASSANT),
    true,
    {{PAWN, E5}},
    {{PAWN, E6}, {PAWN, D5}},
    {ComposeMove(tzcnt(E5), tzcnt(D6), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnPushToPromotion{
    {},
    true,
    {{PAWN, E7}},
    {},
    {ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};
const MoveGenerationTestParameter kWhitePawnPushToPromotionWithCapture{
    {},
    true,
    {{PAWN, D7}},
    {{KING, D8}, {BISHOP, C8}, {KNIGHT, E8}},
    {ComposeMove(tzcnt(D7), tzcnt(C8), PAWN, BISHOP, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(C8), PAWN, BISHOP, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(C8), PAWN, BISHOP, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(C8), PAWN, BISHOP, BISHOP, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(E8), PAWN, KNIGHT, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(E8), PAWN, KNIGHT, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(E8), PAWN, KNIGHT, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D7), tzcnt(E8), PAWN, KNIGHT, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};

// black pawn moves
const MoveGenerationTestParameter kBlackPawnSinglePush{
    {},
    false,
    {},
    {{PAWN, E5}},
    {ComposeMove(tzcnt(E5), tzcnt(E4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)}};
const MoveGenerationTestParameter kBlackPawnBlockedMidfield{{}, false, {{PAWN, D5}}, {{PAWN, D6}}, {}};
const MoveGenerationTestParameter kBlackPawnOnStartRow{
    {},
    false,
    {},
    {{PAWN, G7}},
    {ComposeMove(tzcnt(G7), tzcnt(G6), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH),
     ComposeMove(tzcnt(G7), tzcnt(G5), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)}};
const MoveGenerationTestParameter kBlackPawnBlockedOnStartRow{{}, false, {{PAWN, H6}}, {{PAWN, H7}}, {}};
const MoveGenerationTestParameter kBlackPawnCaptures{
    {},
    false,
    {{PAWN, C4}, {KNIGHT, B4}, {ROOK, D4}},
    {{PAWN, C5}},
    {ComposeMove(tzcnt(C5), tzcnt(B4), PAWN, KNIGHT, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(C5), tzcnt(D4), PAWN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnEnPassantOptionRight{
    (static_cast<Bitboard>(tzcnt(E3)) << BOARD_SHIFT_EN_PASSANT),
    false,
    {{PAWN, E4}, {PAWN, F3}},
    {{PAWN, F4}},
    {ComposeMove(tzcnt(F4), tzcnt(E3), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnEnPassantOptionLeft{
    (static_cast<Bitboard>(tzcnt(G3)) << BOARD_SHIFT_EN_PASSANT),
    false,
    {{PAWN, G4}, {PAWN, F3}},
    {{PAWN, F4}},
    {ComposeMove(tzcnt(F4), tzcnt(G3), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSANT_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnPushToPromotion{
    {},
    false,
    {},
    {{PAWN, A2}},
    {ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};
const MoveGenerationTestParameter kBlackPawnPushToPromotionWithCapture{
    {},
    false,
    {{KING, D1}, {BISHOP, C1}, {KNIGHT, E1}},
    {{PAWN, D2}},
    {ComposeMove(tzcnt(D2), tzcnt(C1), PAWN, BISHOP, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(C1), PAWN, BISHOP, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(C1), PAWN, BISHOP, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(C1), PAWN, BISHOP, BISHOP, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(E1), PAWN, KNIGHT, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(E1), PAWN, KNIGHT, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(E1), PAWN, KNIGHT, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(D2), tzcnt(E1), PAWN, KNIGHT, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};

INSTANTIATE_TEST_SUITE_P(AllAtomicPawnPositions,
                         MoveGenerationTestFixture,
                         ::testing::Values(kWhitePawnSinglePush,
                                           kWhitePawnBlockedMidfield,
                                           kWhitePawnOnStartRow,
                                           kWhitePawnBlockedOnStartRow,
                                           kWhitePawnCaptures,
                                           kWhitePawnEnPassantOptionRight,
                                           kWhitePawnEnPassantOptionLeft,
                                           kWhitePawnPushToPromotion,
                                           kWhitePawnPushToPromotionWithCapture,
                                           kBlackPawnSinglePush,
                                           kBlackPawnBlockedMidfield,
                                           kBlackPawnOnStartRow,
                                           kBlackPawnBlockedOnStartRow,
                                           kBlackPawnCaptures,
                                           kBlackPawnEnPassantOptionRight,
                                           kBlackPawnEnPassantOptionLeft,
                                           kBlackPawnPushToPromotion,
                                           kBlackPawnPushToPromotionWithCapture));

const MoveGenerationTestParameter kWhiteKingAloneMidfield{
    {},
    true,
    {{KING, E4}},
    {},
    {ComposeMove(tzcnt(E4), tzcnt(E5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kWhiteKingInCornerH1{
    {},
    true,
    {{KING, H1}},
    {},
    {ComposeMove(tzcnt(H1), tzcnt(H2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H1), tzcnt(G2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H1), tzcnt(G1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kWhiteKingInCornerA8{
    {},
    true,
    {{KING, A8}},
    {},
    {ComposeMove(tzcnt(A8), tzcnt(B8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A8), tzcnt(B7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A8), tzcnt(A7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

const MoveGenerationTestParameter kBlackKingAloneMidfield{
    {},
    false,
    {},
    {{KING, E4}},
    {ComposeMove(tzcnt(E4), tzcnt(E5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kBlackKingInCornerA1{
    {},
    false,
    {},
    {{KING, A1}},
    {ComposeMove(tzcnt(A1), tzcnt(A2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A1), tzcnt(B2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A1), tzcnt(B1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kBlackKingInCornerH8{
    {},
    false,
    {},
    {{KING, H8}},
    {ComposeMove(tzcnt(H8), tzcnt(H7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H8), tzcnt(G7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H8), tzcnt(G8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kWhiteKingCapture{
    {},
    true,
    {{KING, E4}, {PAWN, E3}},
    {{PAWN, E5}},
    {ComposeMove(tzcnt(E4), tzcnt(D4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E5), KING, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(E4), tzcnt(F5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kBlackKingCapture{
    {},
    false,
    {{PAWN, E3}},
    {{KING, E4}, {PAWN, E5}},
    {ComposeMove(tzcnt(E4), tzcnt(D4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F5), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E3), KING, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(E4), tzcnt(D3), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

INSTANTIATE_TEST_SUITE_P(AllAtomicKingPositions,
                         MoveGenerationTestFixture,
                         ::testing::Values(kWhiteKingAloneMidfield,
                                           kWhiteKingInCornerH1,
                                           kWhiteKingInCornerA8,
                                           kWhiteKingCapture,
                                           kBlackKingAloneMidfield,
                                           kBlackKingInCornerA1,
                                           kBlackKingInCornerH8,
                                           kBlackKingCapture));

const MoveGenerationTestParameter kWhiteRook{
    {},
    true,
    {{ROOK, E4}, {PAWN, E3}},
    {{PAWN, A4}},
    {ComposeMove(tzcnt(E4), tzcnt(E5), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E6), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E7), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E8), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(G4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(H4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(B4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(A4), ROOK, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};

const MoveGenerationTestParameter kBlackRook{
    {},
    false,
    {{PAWN, A4}},
    {{ROOK, E4}, {PAWN, E5}},
    {ComposeMove(tzcnt(E4), tzcnt(E3), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E2), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E1), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(G4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(H4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(B4), ROOK, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(A4), ROOK, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};

INSTANTIATE_TEST_SUITE_P(AllAtomicRookPositions, MoveGenerationTestFixture, ::testing::Values(kWhiteRook, kBlackRook));

const MoveGenerationTestParameter kWhiteBishop{
    {},
    true,
    {{BISHOP, E4}, {PAWN, C5}, {PAWN, G6}},
    {{PAWN, C6}, {PAWN, G7}},
    {ComposeMove(tzcnt(E4), tzcnt(D5), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C6), BISHOP, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(E4), tzcnt(F5), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(G2), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(H1), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D3), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C2), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(B1), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

const MoveGenerationTestParameter kBlackBishop{
    {},
    false,
    {{PAWN, C5}, {PAWN, G6}},
    {{BISHOP, D4}, {PAWN, C6}, {PAWN, G7}},
    {
        ComposeMove(tzcnt(D4), tzcnt(C5), BISHOP, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
        ComposeMove(tzcnt(D4), tzcnt(E5), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(F6), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(E3), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(F2), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(G1), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(C3), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(B2), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
        ComposeMove(tzcnt(D4), tzcnt(A1), BISHOP, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
    }};

INSTANTIATE_TEST_SUITE_P(AllAtomicBishopPositions,
                         MoveGenerationTestFixture,
                         ::testing::Values(kWhiteBishop, kBlackBishop));

const MoveGenerationTestParameter kWhiteQueen{
    {},
    true,
    {{PAWN, A4}, {PAWN, C5}, {PAWN, D2}, {PAWN, G6}, {QUEEN, E4}},
    {{PAWN, A5}, {PAWN, C6}, {PAWN, D3}, {PAWN, G7}},
    {ComposeMove(tzcnt(E4), tzcnt(D5), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C6), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(E4), tzcnt(F5), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F3), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(G2), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(H1), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(D3), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(E4), tzcnt(D4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(C4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(B4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E3), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E2), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E1), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(F4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(G4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(H4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E5), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E6), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E7), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(E4), tzcnt(E8), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

const MoveGenerationTestParameter kBlackQueen{
    {},
    false,
    {{PAWN, A4}, {PAWN, C5}, {PAWN, D2}, {PAWN, G6}},
    {{PAWN, A5}, {PAWN, C6}, {PAWN, D3}, {PAWN, G7}, {QUEEN, D4}},
    {ComposeMove(tzcnt(D4), tzcnt(C5), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(D4), tzcnt(E5), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(F6), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(E3), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(F2), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(G1), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(C3), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(B2), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(A1), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(C4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(B4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(A4), QUEEN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(D4), tzcnt(D5), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(D6), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(D7), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(D8), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(E4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(F4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(G4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(D4), tzcnt(H4), QUEEN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

INSTANTIATE_TEST_SUITE_P(AllAtomicQueenPositions,
                         MoveGenerationTestFixture,
                         ::testing::Values(kWhiteQueen, kBlackQueen));

const MoveGenerationTestParameter kWhiteKnight{
    {},
    true,
    {{PAWN, A2}, {PAWN, D4}, {KNIGHT, B5}},
    {{PAWN, A3}, {PAWN, D5}},
    {ComposeMove(tzcnt(B5), tzcnt(A7), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B5), tzcnt(C7), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B5), tzcnt(D6), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B5), tzcnt(C3), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B5), tzcnt(A3), KNIGHT, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};

const MoveGenerationTestParameter kBlackKnight{
    {},
    false,
    {{PAWN, A2}, {PAWN, D4}},
    {{PAWN, A3}, {PAWN, D5}, {KNIGHT, B4}},
    {ComposeMove(tzcnt(B4), tzcnt(A6), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B4), tzcnt(C6), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B4), tzcnt(D3), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B4), tzcnt(C2), KNIGHT, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(B4), tzcnt(A2), KNIGHT, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};

INSTANTIATE_TEST_SUITE_P(AllAtomicKnightPositions,
                         MoveGenerationTestFixture,
                         ::testing::Values(kWhiteKnight, kBlackKnight));

class CastlingGenerationTestFixture : public MoveGenerationTestFixture
{
};

TEST_P(CastlingGenerationTestFixture, GivenAtomicPosition_ExpectProvidedMovesAreConsidered)
{
    SetUpBoardAccordingToTestParameter();

    const MoveList::iterator returned_move_insertion_iterator = GenerateMoves(position_, move_list_.begin());

    const std::size_t returned_number_of_moves = std::distance(move_list_.begin(), returned_move_insertion_iterator);
    const std::size_t expected_number_of_moves = GetParam().expected_moves.size();
    ASSERT_GE(returned_number_of_moves, expected_number_of_moves) << ToString(move_list_);

    const std::vector<Bitmove> returned_moves(move_list_.begin(), returned_move_insertion_iterator);
    const std::vector<Bitmove>& expected_moves = GetParam().expected_moves;
    EXPECT_THAT(returned_moves, ::testing::IsSupersetOf(expected_moves)) << ToString(move_list_);
}

const MoveGenerationTestParameter kBlackKingSide{
    BOARD_VALUE_CASTLING_BLACK_KINGSIDE,
    false,
    {},
    {{ROOK, H8}, {KING, E8}},
    {ComposeMove(tzcnt(E8), tzcnt(G8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_KINGSIDE_CASTLING)}};
const MoveGenerationTestParameter kBlackQueenSide{
    BOARD_VALUE_CASTLING_BLACK_QUEENSIDE,
    false,
    {},
    {{ROOK, A8}, {KING, E8}},
    {ComposeMove(tzcnt(E8), tzcnt(C8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUEENSIDE_CASTLING)}};
const MoveGenerationTestParameter kWhiteKingSide{
    BOARD_VALUE_CASTLING_WHITE_KINGSIDE,
    true,
    {{ROOK, H1}, {KING, E1}},
    {},
    {ComposeMove(tzcnt(E1), tzcnt(G1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_KINGSIDE_CASTLING)}};
const MoveGenerationTestParameter kWhiteQueenSide{
    BOARD_VALUE_CASTLING_WHITE_QUEENSIDE,
    true,
    {{ROOK, A1}, {KING, E1}},
    {},
    {ComposeMove(tzcnt(E1), tzcnt(C1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUEENSIDE_CASTLING)}};

INSTANTIATE_TEST_SUITE_P(AtomicCastlingPositions,
                         CastlingGenerationTestFixture,
                         ::testing::Values(kBlackKingSide, kBlackQueenSide, kWhiteKingSide, kWhiteQueenSide));

MATCHER(IsACastlingMove, "")
{
    const Bitmove move_type = arg & MOVE_MASK_TYPE;
    return (move_type == MOVE_VALUE_TYPE_KINGSIDE_CASTLING) || (move_type == MOVE_VALUE_TYPE_QUEENSIDE_CASTLING);
}

class NoCastlingGenerationTestFixture : public MoveGenerationTestFixture
{
};

TEST_P(NoCastlingGenerationTestFixture, GivenAtomicPosition_ExpectExcludesAnyCastlingMove)
{
    SetUpBoardAccordingToTestParameter();
    std::ignore = GetParam().expected_moves;  // not used

    const MoveList::iterator returned_move_insertion_iterator = GenerateMoves(position_, move_list_.begin());

    const std::vector<Bitmove> returned_moves(move_list_.begin(), returned_move_insertion_iterator);
    EXPECT_THAT(returned_moves, ::testing::Not(::testing::Contains(IsACastlingMove()))) << ToString(move_list_);
}

const MoveGenerationTestParameter kBlackKingSideBlocked{BOARD_VALUE_CASTLING_BLACK_KINGSIDE,
                                                        false,
                                                        {{KNIGHT, F8}},
                                                        {{ROOK, H8}, {KING, E8}},
                                                        {/*unused*/}};
const MoveGenerationTestParameter kBlackQueenSideBlocked{BOARD_VALUE_CASTLING_BLACK_QUEENSIDE,
                                                         false,
                                                         {{KNIGHT, C8}},
                                                         {{ROOK, A8}, {KING, E8}},
                                                         {/*unused*/}};
const MoveGenerationTestParameter kWhiteKingSideBlocked{BOARD_VALUE_CASTLING_WHITE_KINGSIDE,
                                                        true,
                                                        {{ROOK, H1}, {KING, E1}},
                                                        {{KNIGHT, G1}},
                                                        {/*unused*/}};
const MoveGenerationTestParameter kWhiteQueenSideBlocked{BOARD_VALUE_CASTLING_WHITE_QUEENSIDE,
                                                         true,
                                                         {{ROOK, A1}, {KING, E1}},
                                                         {{KNIGHT, B1}},
                                                         {/*unused*/}};
const MoveGenerationTestParameter kBlackKingSideNoRights{{}, false, {}, {{ROOK, H8}, {KING, E8}}, {/*unused*/}};
const MoveGenerationTestParameter kBlackQueenSideNoRights{{}, false, {}, {{ROOK, A8}, {KING, E8}}, {/*unused*/}};
const MoveGenerationTestParameter kWhiteKingSideNoRights{{}, true, {{ROOK, H1}, {KING, E1}}, {}, {/*unused*/}};
const MoveGenerationTestParameter kWhiteQueenSideNoRights{{}, true, {{ROOK, A1}, {KING, E1}}, {}, {/*unused*/}};

INSTANTIATE_TEST_SUITE_P(AtomicNoCastlingPositions,
                         NoCastlingGenerationTestFixture,
                         ::testing::Values(kBlackKingSideBlocked,
                                           kBlackQueenSideBlocked,
                                           kWhiteKingSideBlocked,
                                           kWhiteQueenSideBlocked,
                                           kBlackKingSideNoRights,
                                           kBlackQueenSideNoRights,
                                           kWhiteKingSideNoRights,
                                           kWhiteQueenSideNoRights));

}  // namespace
}  // namespace Chess
