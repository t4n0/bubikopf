#include "bitboard/position.h"

#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>

namespace Chess {
namespace {

bool BoardsEqualTheirIndexExceptIgnored(
    const PositionWithBitboards& position,
    const std::vector<std::size_t> indexes_to_ignore) {
  bool return_value = true;
  for (std::size_t index{0}; index < position.boards_.size(); index++) {
    if (!std::any_of(begin(indexes_to_ignore), end(indexes_to_ignore),
                     [&index](auto elem) { return elem == index; })) {
      if (position[index] != index) {
        return_value = false;
      }
    }
  }
  return return_value;
}

TEST(
    PositionMakeMoveFixture,
    GivenAnyMove_ExpectSideSwitchedAndHistoryAppendedAndEnPassentClearedAndAttackingPieceMoved) {
  // Setup
  const bitboard_t source = C3;
  const bitboard_t target = E5;
  const bitboard_t extras = BOARD_MASK_WHITE_TURN;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, source, 10, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t any_move = source_bit | (target_bit << MOVE_SHIFT_TARGET) |
                          (BISHOP << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(any_move);

  // Expect
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN);
  EXPECT_TRUE(position.extras_history_insertion_index_ == 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT);
  EXPECT_EQ(position[BOARD_IDX_WHITE], target);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 12}));
}

TEST(
    PositionMakeMoveFixture,
    GivenQuietNonPawn_ExpectAttackingPieceMovedAndIncrementedStaticPliesCount) {
  // Setup
  const bitboard_t source = H1;
  const bitboard_t target = G3;
  const bitboard_t static_plies = 7;
  const bitboard_t extras = BOARD_MASK_WHITE_TURN | static_plies;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, source, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t quiet_non_pawn_move =
      MOVE_VALUE_TYPE_QUIET_NON_PAWN | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KNIGHT << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(quiet_non_pawn_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + KNIGHT], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 11}));
}

TEST(
    PositionMakeMoveFixture,
    GivenCapture_ExpectAttackingPieceMovedAndHarmedPieceRemovedAndStaticPliesCountReset) {
  // Setup
  const bitboard_t static_plies = 8;
  const bitboard_t source = D1;
  const bitboard_t target = D5;
  const bitboard_t extras = static_plies;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, 5, source, 7, 8, target,
                                  target, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t capture_move =
      MOVE_VALUE_TYPE_CAPTURE | source_bit | (target_bit << MOVE_SHIFT_TARGET) |
      (QUEEN << MOVE_SHIFT_MOVED_PIECE) | (PAWN << MOVE_SHIFT_CAPTURED_PIECE);

  // Call
  position.MakeMove(capture_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + QUEEN], target);
  EXPECT_EQ(position[BOARD_IDX_WHITE], 0);
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], 0);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 6, 9, 10}));
}

TEST(PositionMakeMoveFixture,
     GivenPawnPush_ExpectAttackingPieceMovedAndStaticPliesCountReset) {
  // Setup
  const bitboard_t source = F2;
  const bitboard_t target = F3;
  const bitboard_t static_plies = 9;
  const bitboard_t extras = static_plies;
  PositionWithBitboards position{{extras, source, source, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t single_push = MOVE_VALUE_TYPE_PAWN_PUSH | source_bit |
                             (target_bit << MOVE_SHIFT_TARGET) |
                             (PAWN << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(single_push);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2}));
}

TEST(
    PositionMakeMoveFixture,
    GivenPawnDoublePush_ExpectAttackingPieceMovedAndStaticPliesCountResetAndEnpassentSet) {
  // Setup
  const bitboard_t source = F2;
  const bitboard_t target = F4;
  const bitboard_t static_plies = 10;
  const bitboard_t extras = BOARD_MASK_WHITE_TURN | static_plies;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, source, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t en_passent_bit = tzcnt(F3);
  const move_t double_push = MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH | source_bit |
                             (target_bit << MOVE_SHIFT_TARGET) |
                             (PAWN << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(double_push);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_EQ((position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT) >>
                BOARD_SHIFT_EN_PASSENT,
            en_passent_bit);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 10}));
}

TEST(
    PositionMakeMoveFixture,
    GivenEnPassentCapture_ExpectAttackingPieceMovedAndHarmedPawnRemovedAndStaticPliesCountReset) {
  // Setup
  const bitboard_t source = H6;
  const bitboard_t target = G7;
  const bitboard_t harmed = G6;
  const bitboard_t harmed_bit = tzcnt(harmed);
  const bitboard_t static_plies = 11;
  const bitboard_t extras = BOARD_MASK_WHITE_TURN | static_plies |
                            (harmed_bit << BOARD_SHIFT_EN_PASSENT);
  PositionWithBitboards position{{extras, harmed, harmed, 3, 4, 5, 6, 7, 8, 9,
                                  source, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t en_passent_capture =
      MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (PAWN << MOVE_SHIFT_MOVED_PIECE) |
      (PAWN << MOVE_SHIFT_CAPTURED_PIECE);

  // Call
  position.MakeMove(en_passent_capture);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_BLACK], 0);
  EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], 0);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);

  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2, 9, 10}));
}

TEST(
    PositionMakeMoveFixture,
    GivenKingsideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked) {
  // Setup
  const bitboard_t source_king = E1;
  const bitboard_t target_king = G1;
  const bitboard_t source_rook = H1;
  const bitboard_t target_rook = F1;
  const bitboard_t static_plies = 12;
  const bitboard_t extras = static_plies | BOARD_MASK_WHITE_TURN |
                            BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                  source_rook, 14, source_king, 16}};

  const move_t source_bit = tzcnt(source_king);
  const move_t target_bit = tzcnt(target_king);
  const move_t kingside_castling =
      MOVE_VALUE_TYPE_KINGSIDE_CASTLING | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(kingside_castling);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + KING], target_king);
  EXPECT_EQ(position[BOARD_IDX_WHITE + ROOK], target_rook);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_WHITE_KINGSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 13, 15}));
}

TEST(
    PositionMakeMoveFixture,
    GivenQueensideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked) {
  // Setup
  const bitboard_t source_king = E8;
  const bitboard_t target_king = C8;
  const bitboard_t source_rook = A8;
  const bitboard_t target_rook = D8;
  const bitboard_t static_plies = 13;
  const bitboard_t extras = static_plies | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, source_rook, 6,
                                  source_king, 8, 9, 10, 11, 12, 13, 14, 15,
                                  16}};

  const move_t source_bit = tzcnt(source_king);
  const move_t target_bit = tzcnt(target_king);
  const move_t queenside_castling =
      MOVE_VALUE_TYPE_QUEENSIDE_CASTLING | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(queenside_castling);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + KING], target_king);
  EXPECT_EQ(position[BOARD_IDX_BLACK + ROOK], target_rook);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 5, 7}));
}

TEST(
    PositionMakeMoveFixture,
    GivenKingsideRook_ExpectPieceMovedAndIncrementedStaticPliesCountAndCastlingRightsRevoked) {
  // Setup
  const bitboard_t source = H1;
  const bitboard_t target = G1;
  const bitboard_t static_plies = 20;
  const bitboard_t extras = BOARD_MASK_WHITE_TURN | static_plies |
                            BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, source, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t kingside_rook_move = MOVE_VALUE_TYPE_KINGSIDE_ROOK | source_bit |
                                    (target_bit << MOVE_SHIFT_TARGET) |
                                    (ROOK << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(kingside_rook_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + ROOK], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_WHITE_KINGSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 13}));
}

TEST(
    PositionMakeMoveFixture,
    GivenQueensideRook_ExpectPieceMovedAndIncrementedStaticPliesCountAndCastlingRightsRevoked) {
  // Setup
  const bitboard_t source = H1;
  const bitboard_t target = G1;
  const bitboard_t static_plies = 20;
  const bitboard_t extras = static_plies | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, source, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t queenside_rook_move =
      MOVE_VALUE_TYPE_QUEENSIDE_ROOK | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (ROOK << MOVE_SHIFT_MOVED_PIECE);

  // Call
  position.MakeMove(queenside_rook_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + ROOK], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 5}));
}

TEST(PositionMakeMoveFixture,
     GivenPromotion_ExpectPieceMovedAndStaticPliesReset) {
  // Setup
  const bitboard_t source = H2;
  const bitboard_t target = H1;
  const bitboard_t static_plies = 30;
  const bitboard_t extras = static_plies;
  PositionWithBitboards position{
      {extras, 1, source, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t promotion_move = MOVE_VALUE_TYPE_PROMOTION | source_bit |
                                (target_bit << MOVE_SHIFT_TARGET) |
                                (PAWN << MOVE_SHIFT_MOVED_PIECE) |
                                (QUEEN << MOVE_SHIFT_PROMOTION);

  // Call
  position.MakeMove(promotion_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_BLACK + QUEEN], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2, 6}));
}

TEST(PositionMakeMoveFixture,
     GivenPromotionCapture_ExpectPieceMovedAndStaticPliesReset) {
  // Setup
  const bitboard_t source = B7;
  const bitboard_t target = B8;
  const bitboard_t static_plies = 31;
  const bitboard_t extras = static_plies | BOARD_MASK_WHITE_TURN;
  PositionWithBitboards position{
      {extras, 1, 2, 3, target, 5, 6, 7, 8, 9, source, 11, 12, 13, 0, 15, 16}};

  const move_t source_bit = tzcnt(source);
  const move_t target_bit = tzcnt(target);
  const move_t promotion_capture_move =
      MOVE_VALUE_TYPE_PROMOTION | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (PAWN << MOVE_SHIFT_MOVED_PIECE) |
      (QUEEN << MOVE_SHIFT_PROMOTION) | (BISHOP << MOVE_SHIFT_CAPTURED_PIECE);

  // Call
  position.MakeMove(promotion_capture_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_WHITE + QUEEN], target);
  EXPECT_EQ(position[BOARD_IDX_BLACK + BISHOP], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 4, 9, 10, 14}));
}

}  // namespace
}  // namespace Chess
