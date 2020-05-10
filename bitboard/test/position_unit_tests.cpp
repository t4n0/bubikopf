#include "bitboard/position.h"

#include "bitboard/squares.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>

namespace Chess {
namespace {

TEST(BoardIndxesTest,
     GivenUniqueNumberToEachBitboard_ExpectRespectiveUniqueNumber) {
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
  for (Bitboard board : unit.boards_) {
    EXPECT_EQ(board, expected_unique_number);
    expected_unique_number++;
  }
}

TEST(PositionWhiteToMove, GivenWhitesTurn_ExpectTrue) {
  // Setup
  PositionWithBitboards position{};
  position.boards_[BOARD_IDX_EXTRAS] = BOARD_ZEROS;
  position.boards_[BOARD_IDX_EXTRAS] |= BOARD_MASK_WHITE_TURN;

  // Call & Expect
  EXPECT_TRUE(position.WhiteToMove());
}

TEST(PositionWhiteToMove, GivenBlacksTurn_ExpectFalse) {
  // Setup
  PositionWithBitboards position{};
  position.boards_[BOARD_IDX_EXTRAS] = BOARD_ONES;
  position.boards_[BOARD_IDX_EXTRAS] &= ~BOARD_MASK_WHITE_TURN;

  // Call & Expect
  EXPECT_FALSE(position.WhiteToMove());
}

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
  const Bitboard source = C3;
  const Bitboard target = E5;
  const Bitboard extras = BOARD_MASK_WHITE_TURN;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, source, 10, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove any_move = source_bit | (target_bit << MOVE_SHIFT_TARGET) |
                           (BISHOP << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(any_move);

  // Expect
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_WHITE_TURN);
  EXPECT_TRUE(position.extras_history_insertion_index_ == 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT);
  EXPECT_EQ(position[BOARD_IDX_WHITE], target);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 12}));

  // Call
  position.UnmakeMove(any_move);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenQuietNonPawn_ExpectAttackingPieceMovedAndIncrementedStaticPliesCount) {
  // Setup
  const Bitboard source = H1;
  const Bitboard target = G3;
  const Bitboard static_plies = 7;
  const Bitboard extras = BOARD_MASK_WHITE_TURN | static_plies;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, source, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove quiet_non_pawn_move =
      MOVE_VALUE_TYPE_QUIET_NON_PAWN | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KNIGHT << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(quiet_non_pawn_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + KNIGHT], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 11}));

  // Call
  position.UnmakeMove(quiet_non_pawn_move);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenCapture_ExpectAttackingPieceMovedAndHarmedPieceRemovedAndStaticPliesCountReset) {
  // Setup
  const Bitboard static_plies = 8;
  const Bitboard source = D1;
  const Bitboard target = D5;
  const Bitboard extras = static_plies;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, 5, source, 7, 8, target,
                                  target, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove capture_move =
      MOVE_VALUE_TYPE_CAPTURE | source_bit | (target_bit << MOVE_SHIFT_TARGET) |
      (QUEEN << MOVE_SHIFT_MOVED_PIECE) | (PAWN << MOVE_SHIFT_CAPTURED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(capture_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + QUEEN], target);
  EXPECT_EQ(position[BOARD_IDX_WHITE], 0);
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], 0);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 6, 9, 10}));

  // Call
  position.UnmakeMove(capture_move);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(PositionMakeMoveFixture,
     GivenPawnPush_ExpectAttackingPieceMovedAndStaticPliesCountReset) {
  // Setup
  const Bitboard source = F2;
  const Bitboard target = F3;
  const Bitboard static_plies = 9;
  const Bitboard extras = static_plies;
  PositionWithBitboards position{{extras, source, source, 3, 4, 5, 6, 7, 8, 9,
                                  10, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove single_push = MOVE_VALUE_TYPE_PAWN_PUSH | source_bit |
                              (target_bit << MOVE_SHIFT_TARGET) |
                              (PAWN << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(single_push);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2}));

  // Call
  position.UnmakeMove(single_push);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenPawnDoublePush_ExpectAttackingPieceMovedAndStaticPliesCountResetAndEnpassentSet) {
  // Setup
  const Bitboard source = F2;
  const Bitboard target = F4;
  const Bitboard static_plies = 10;
  const Bitboard extras = BOARD_MASK_WHITE_TURN | static_plies;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, source, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove en_passent_bit = tzcnt(F3);
  const Bitmove double_push = MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH | source_bit |
                              (target_bit << MOVE_SHIFT_TARGET) |
                              (PAWN << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(double_push);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_EQ((position[BOARD_IDX_EXTRAS] & BOARD_MASK_EN_PASSENT) >>
                BOARD_SHIFT_EN_PASSENT,
            en_passent_bit);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 10}));

  // Call
  position.UnmakeMove(double_push);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenEnPassentCapture_ExpectAttackingPieceMovedAndHarmedPawnRemovedAndStaticPliesCountReset) {
  // Setup
  const Bitboard source = H6;
  const Bitboard target = G7;
  const Bitboard harmed = G6;
  const Bitboard harmed_bit = tzcnt(harmed);
  const Bitboard static_plies = 11;
  const Bitboard extras = BOARD_MASK_WHITE_TURN | static_plies |
                          (harmed_bit << BOARD_SHIFT_EN_PASSENT);
  PositionWithBitboards position{{extras, harmed, harmed, 3, 4, 5, 6, 7, 8, 9,
                                  source, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove en_passent_capture =
      MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (PAWN << MOVE_SHIFT_MOVED_PIECE) |
      (PAWN << MOVE_SHIFT_CAPTURED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(en_passent_capture);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], target);
  EXPECT_EQ(position[BOARD_IDX_BLACK], 0);
  EXPECT_EQ(position[BOARD_IDX_BLACK + PAWN], 0);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);

  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 2, 9, 10}));

  // Call
  position.UnmakeMove(en_passent_capture);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenKingsideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked) {
  // Setup
  const Bitboard source_king = E1;
  const Bitboard target_king = G1;
  const Bitboard source_rook = H1;
  const Bitboard target_rook = F1;
  const Bitboard static_plies = 12;
  const Bitboard extras = static_plies | BOARD_MASK_WHITE_TURN |
                          BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                  source_rook, 14, source_king, 16}};

  const Bitmove source_bit = tzcnt(source_king);
  const Bitmove target_bit = tzcnt(target_king);
  const Bitmove kingside_castling =
      MOVE_VALUE_TYPE_KINGSIDE_CASTLING | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

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

  // Call
  position.UnmakeMove(kingside_castling);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenQueensideCastling_ExpectKingAndRookMovedAndStaticPliesIncrementedAndCastlingRightsRevoked) {
  // Setup
  const Bitboard source_king = E8;
  const Bitboard target_king = C8;
  const Bitboard source_rook = A8;
  const Bitboard target_rook = D8;
  const Bitboard static_plies = 13;
  const Bitboard extras = static_plies | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
  PositionWithBitboards position{{extras, 1, 2, 3, 4, source_rook, 6,
                                  source_king, 8, 9, 10, 11, 12, 13, 14, 15,
                                  16}};

  const Bitmove source_bit = tzcnt(source_king);
  const Bitmove target_bit = tzcnt(target_king);
  const Bitmove queenside_castling =
      MOVE_VALUE_TYPE_QUEENSIDE_CASTLING | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (KING << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

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

  // Call
  position.UnmakeMove(queenside_castling);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenKingsideRook_ExpectPieceMovedAndIncrementedStaticPliesCountAndCastlingRightsRevoked) {
  // Setup
  const Bitboard source = H1;
  const Bitboard target = G1;
  const Bitboard static_plies = 20;
  const Bitboard extras = BOARD_MASK_WHITE_TURN | static_plies |
                          BOARD_VALUE_CASTLING_WHITE_KINGSIDE;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, source, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove kingside_rook_move =
      MOVE_VALUE_TYPE_KINGSIDE_ROOK | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (ROOK << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(kingside_rook_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + ROOK], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_WHITE_KINGSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 9, 13}));

  // Call
  position.UnmakeMove(kingside_rook_move);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(
    PositionMakeMoveFixture,
    GivenQueensideRook_ExpectPieceMovedAndIncrementedStaticPliesCountAndCastlingRightsRevoked) {
  // Setup
  const Bitboard source = H1;
  const Bitboard target = G1;
  const Bitboard static_plies = 20;
  const Bitboard extras = static_plies | BOARD_VALUE_CASTLING_BLACK_QUEENSIDE;
  PositionWithBitboards position{
      {extras, 1, 2, 3, 4, source, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove queenside_rook_move =
      MOVE_VALUE_TYPE_QUEENSIDE_ROOK | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (ROOK << MOVE_SHIFT_MOVED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(queenside_rook_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_BLACK + ROOK], target);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES,
            static_plies + 1);
  EXPECT_FALSE(position[BOARD_IDX_EXTRAS] &
               BOARD_VALUE_CASTLING_BLACK_QUEENSIDE);
  EXPECT_TRUE(BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 5}));

  // Call
  position.UnmakeMove(queenside_rook_move);

  // Expect
  EXPECT_EQ(position, position_prior_make_move);
}

TEST(PositionMakeMoveFixture,
     GivenPromotion_ExpectPieceMovedAndStaticPliesReset) {
  // Setup
  const Bitboard source = H2;
  const Bitboard target = H1;
  const Bitboard static_plies = 30;
  const Bitboard extras = static_plies;
  PositionWithBitboards position{
      {extras, 1, source, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove promotion_move = MOVE_VALUE_TYPE_PROMOTION | source_bit |
                                 (target_bit << MOVE_SHIFT_TARGET) |
                                 (PAWN << MOVE_SHIFT_MOVED_PIECE) |
                                 (QUEEN << MOVE_SHIFT_PROMOTION);

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

TEST(PositionMakeMoveFixture,
     GivenPromotionCapture_ExpectPieceMovedAndStaticPliesReset) {
  // Setup
  const Bitboard source = B7;
  const Bitboard target = B8;
  const Bitboard static_plies = 31;
  const Bitboard extras = static_plies | BOARD_MASK_WHITE_TURN;
  PositionWithBitboards position{{extras, target, 2, 3, target, 5, 6, 7, 8, 9,
                                  source, 11, 12, 13, BOARD_ZEROS, 15, 16}};

  const Bitmove source_bit = tzcnt(source);
  const Bitmove target_bit = tzcnt(target);
  const Bitmove promotion_capture_move =
      MOVE_VALUE_TYPE_PROMOTION | source_bit |
      (target_bit << MOVE_SHIFT_TARGET) | (PAWN << MOVE_SHIFT_MOVED_PIECE) |
      (QUEEN << MOVE_SHIFT_PROMOTION) | (BISHOP << MOVE_SHIFT_CAPTURED_PIECE);

  const PositionWithBitboards position_prior_make_move{position};

  // Call
  position.MakeMove(promotion_capture_move);

  // Expect
  EXPECT_EQ(position[BOARD_IDX_WHITE + PAWN], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_WHITE + QUEEN], target);
  EXPECT_EQ(position[BOARD_IDX_BLACK + BISHOP], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_BLACK], BOARD_ZEROS);
  EXPECT_EQ(position[BOARD_IDX_EXTRAS] & BOARD_MASK_STATIC_PLIES, 0);
  EXPECT_TRUE(
      BoardsEqualTheirIndexExceptIgnored(position, {0, 1, 4, 9, 10, 14}));

  // Call
  position.UnmakeMove(promotion_capture_move);

  // Expect
  EXPECT_EQ(position.boards_, position_prior_make_move.boards_);
  EXPECT_EQ(position, position_prior_make_move);
}

}  // namespace
}  // namespace Chess
