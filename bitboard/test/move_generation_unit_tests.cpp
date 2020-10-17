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
    std::vector<std::pair<std::size_t, Bitboard>> white_pieces;
    std::vector<std::pair<std::size_t, Bitboard>> black_pieces;
    std::vector<Bitmove> expected_moves;
};

class MoveGenerationTestFixture : public ::testing::TestWithParam<MoveGenerationTestParameter>
{
  public:
    void SetUp() override { position_[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING; }
    void TearDown() override {}

    PositionWithBitboards position_{};
    MoveList move_list_{};
};

TEST_P(MoveGenerationTestFixture, GivenAtomicPosition_ExpectAllPseudoLegalMoves)
{
    position_[BOARD_IDX_EXTRAS] |= GetParam().extras;

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

    const MoveList::iterator returned_move_insertion_iterator = GenerateMoves(position_, move_list_.begin());

    const std::size_t expected_number_of_moves = GetParam().expected_moves.size();
    ASSERT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator), expected_number_of_moves);
    std::vector<Bitmove> returned_moves{};
    returned_moves.resize(expected_number_of_moves);
    std::copy_n(move_list_.begin(), expected_number_of_moves, returned_moves.begin());

    EXPECT_THAT(returned_moves, ::testing::UnorderedElementsAreArray(GetParam().expected_moves));
}

// white pawn moves
const MoveGenerationTestParameter kWhitePawnSinglePush{
    BOARD_MASK_WHITE_TURN,
    {{PAWN, E5}},
    {},
    {ComposeMove(tzcnt(E5), tzcnt(E6), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)}};
const MoveGenerationTestParameter kWhitePawnBlockedMidfield{BOARD_MASK_WHITE_TURN, {{PAWN, D5}}, {{PAWN, D6}}, {}};
const MoveGenerationTestParameter kWhitePawnOnStartRow{
    BOARD_MASK_WHITE_TURN,
    {{PAWN, A2}},
    {},
    {ComposeMove(tzcnt(A2), tzcnt(A3), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH),
     ComposeMove(tzcnt(A2), tzcnt(A4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)}};
const MoveGenerationTestParameter kWhitePawnBlockedOnStartRow{BOARD_MASK_WHITE_TURN, {{PAWN, C2}}, {{PAWN, C3}}, {}};
const MoveGenerationTestParameter kWhitePawnCaptures{
    BOARD_MASK_WHITE_TURN,
    {{PAWN, C5}},
    {{PAWN, C6}, {KNIGHT, B6}, {ROOK, D6}},
    {ComposeMove(tzcnt(C5), tzcnt(B6), PAWN, KNIGHT, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(C5), tzcnt(D6), PAWN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnEnPassentOptionRight{
    BOARD_MASK_WHITE_TURN | (static_cast<Bitboard>(tzcnt(F6)) << BOARD_SHIFT_EN_PASSENT),
    {{PAWN, E5}},
    {{PAWN, E6}, {PAWN, F5}},
    {ComposeMove(tzcnt(E5), tzcnt(F6), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnEnPassentOptionLeft{
    BOARD_MASK_WHITE_TURN | (static_cast<Bitboard>(tzcnt(D6)) << BOARD_SHIFT_EN_PASSENT),
    {{PAWN, E5}},
    {{PAWN, E6}, {PAWN, D5}},
    {ComposeMove(tzcnt(E5), tzcnt(D6), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE)}};
const MoveGenerationTestParameter kWhitePawnPushToPromotion{
    BOARD_MASK_WHITE_TURN,
    {{PAWN, E7}},
    {},
    {ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};
const MoveGenerationTestParameter kWhitePawnPushToPromotionWithCapture{
    BOARD_MASK_WHITE_TURN,
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
    BOARD_MASK_BLACK_TURN,
    {},
    {{PAWN, E5}},
    {ComposeMove(tzcnt(E5), tzcnt(E4), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH)}};
const MoveGenerationTestParameter kBlackPawnBlockedMidfield{BOARD_MASK_BLACK_TURN, {{PAWN, D5}}, {{PAWN, D6}}, {}};
const MoveGenerationTestParameter kBlackPawnOnStartRow{
    BOARD_MASK_BLACK_TURN,
    {},
    {{PAWN, G7}},
    {ComposeMove(tzcnt(G7), tzcnt(G6), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_PUSH),
     ComposeMove(tzcnt(G7), tzcnt(G5), PAWN, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)}};
const MoveGenerationTestParameter kBlackPawnBlockedOnStartRow{BOARD_MASK_BLACK_TURN, {{PAWN, H6}}, {{PAWN, H7}}, {}};
const MoveGenerationTestParameter kBlackPawnCaptures{
    BOARD_MASK_BLACK_TURN,
    {{PAWN, C4}, {KNIGHT, B4}, {ROOK, D4}},
    {{PAWN, C5}},
    {ComposeMove(tzcnt(C5), tzcnt(B4), PAWN, KNIGHT, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE),
     ComposeMove(tzcnt(C5), tzcnt(D4), PAWN, ROOK, NO_PROMOTION, MOVE_VALUE_TYPE_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnEnPassentOptionRight{
    BOARD_MASK_BLACK_TURN | (static_cast<Bitboard>(tzcnt(E3)) << BOARD_SHIFT_EN_PASSENT),
    {{PAWN, E4}, {PAWN, F3}},
    {{PAWN, F4}},
    {ComposeMove(tzcnt(F4), tzcnt(E3), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnEnPassentOptionLeft{
    BOARD_MASK_BLACK_TURN | (static_cast<Bitboard>(tzcnt(G3)) << BOARD_SHIFT_EN_PASSENT),
    {{PAWN, G4}, {PAWN, F3}},
    {{PAWN, F4}},
    {ComposeMove(tzcnt(F4), tzcnt(G3), PAWN, PAWN, NO_PROMOTION, MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE)}};
const MoveGenerationTestParameter kBlackPawnPushToPromotion{
    BOARD_MASK_BLACK_TURN,
    {},
    {{PAWN, A2}},
    {ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, QUEEN, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, ROOK, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, KNIGHT, MOVE_VALUE_TYPE_PROMOTION),
     ComposeMove(tzcnt(A2), tzcnt(A1), PAWN, NO_CAPTURE, BISHOP, MOVE_VALUE_TYPE_PROMOTION)}};
const MoveGenerationTestParameter kBlackPawnPushToPromotionWithCapture{
    BOARD_MASK_BLACK_TURN,
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
                         ::testing::ValuesIn({kWhitePawnSinglePush,
                                              kWhitePawnBlockedMidfield,
                                              kWhitePawnOnStartRow,
                                              kWhitePawnBlockedOnStartRow,
                                              kWhitePawnCaptures,
                                              kWhitePawnEnPassentOptionRight,
                                              kWhitePawnEnPassentOptionLeft,
                                              kWhitePawnPushToPromotion,
                                              kWhitePawnPushToPromotionWithCapture,
                                              kBlackPawnSinglePush,
                                              kBlackPawnBlockedMidfield,
                                              kBlackPawnOnStartRow,
                                              kBlackPawnBlockedOnStartRow,
                                              kBlackPawnCaptures,
                                              kBlackPawnEnPassentOptionRight,
                                              kBlackPawnEnPassentOptionLeft,
                                              kBlackPawnPushToPromotion,
                                              kBlackPawnPushToPromotionWithCapture}));

const MoveGenerationTestParameter kWhiteKingAloneMidfield{
    BOARD_MASK_WHITE_TURN,
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
    BOARD_MASK_WHITE_TURN,
    {{KING, H1}},
    {},
    {ComposeMove(tzcnt(H1), tzcnt(H2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H1), tzcnt(G2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H1), tzcnt(G1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kWhiteKingInCornerA8{
    BOARD_MASK_WHITE_TURN,
    {{KING, A8}},
    {},
    {ComposeMove(tzcnt(A8), tzcnt(B8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A8), tzcnt(B7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A8), tzcnt(A7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};

const MoveGenerationTestParameter kBlackKingAloneMidfield{
    BOARD_MASK_BLACK_TURN,
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
    BOARD_MASK_BLACK_TURN,
    {},
    {{KING, A1}},
    {ComposeMove(tzcnt(A1), tzcnt(A2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A1), tzcnt(B2), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(A1), tzcnt(B1), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kBlackKingInCornerH8{
    BOARD_MASK_BLACK_TURN,
    {},
    {{KING, H8}},
    {ComposeMove(tzcnt(H8), tzcnt(H7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H8), tzcnt(G7), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN),
     ComposeMove(tzcnt(H8), tzcnt(G8), KING, NO_CAPTURE, NO_PROMOTION, MOVE_VALUE_TYPE_QUIET_NON_PAWN)}};
const MoveGenerationTestParameter kWhiteKingCapture{
    BOARD_MASK_WHITE_TURN,
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
    BOARD_MASK_BLACK_TURN,
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
                         ::testing::ValuesIn({kWhiteKingAloneMidfield,
                                              kWhiteKingInCornerH1,
                                              kWhiteKingInCornerA8,
                                              kWhiteKingCapture,
                                              kBlackKingAloneMidfield,
                                              kBlackKingInCornerA1,
                                              kBlackKingInCornerH8,
                                              kBlackKingCapture}));

}  // namespace
}  // namespace Chess
