#include "bitboard/move_generation.h"
#include "bitboard/squares.h"
#include "hardware/population_count.h"
#include "hardware/trailing_zeros_count.h"

#include <gmock/gmock.h>

namespace Chess {
namespace {

void CheckMove(const Bitmove move, const Bitboard source, const Bitboard target,
               const Bitmove moved_piece, const Bitmove captured_piece,
               const Bitmove promotion, const Bitmove move_type) {
  EXPECT_EQ(move & MOVE_MASK_SOURCE, tzcnt(source));
  EXPECT_EQ((move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET, tzcnt(target));
  EXPECT_EQ((move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE,
            moved_piece);
  EXPECT_EQ((move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE,
            captured_piece);
  EXPECT_EQ((move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION, promotion);
  EXPECT_EQ(move & MOVE_MASK_TYPE, move_type);
}

class WhitePawnMoveGenerationFixture : public ::testing::Test {
 public:
  void SetUp() override {
    position_[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING | BOARD_MASK_WHITE_TURN;
  }

  PositionWithBitboards position_{};
  MoveList move_list_{};
};

TEST_F(WhitePawnMoveGenerationFixture,
       GivenFreeSquareInfrontAndNotOnStartingRow_ExpectSinglePawnPush) {
  position_[BOARD_IDX_WHITE] = F5;
  position_[BOARD_IDX_WHITE + PAWN] = F5;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());
  const Bitmove& returned_move = *move_list_.begin();

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  CheckMove(returned_move, F5, F6, PAWN, NO_CAPTURE, NO_PROMOTION,
            MOVE_VALUE_TYPE_PAWN_PUSH);
}

TEST_F(WhitePawnMoveGenerationFixture, GivenNoFreeSquareInfront_ExpectNoMove) {
  position_[BOARD_IDX_WHITE] = F6;
  position_[BOARD_IDX_WHITE + PAWN] = F6;
  position_[BOARD_IDX_BLACK] = F7;
  position_[BOARD_IDX_BLACK + PAWN] = F7;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            0);
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenOnStartingRowWithTwoFreeSquaresInfront_ExpectDoublePawnPush) {
  position_[BOARD_IDX_WHITE] = E2;
  position_[BOARD_IDX_WHITE + PAWN] = E2;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            2);
  std::array<Bitmove, 2> returned_moves{};
  std::copy_n(move_list_.begin(), 2, returned_moves.begin());

  EXPECT_THAT(returned_moves,
              ::testing::Contains(
                  ComposeMove(tzcnt(E2), tzcnt(E4), PAWN, NO_CAPTURE,
                              NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)));
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenOnStartingRowWithNoFreeSquaresInfront_ExpectNoMove) {
  position_[BOARD_IDX_WHITE] = A2;
  position_[BOARD_IDX_WHITE + PAWN] = A2;
  position_[BOARD_IDX_BLACK] = A3;
  position_[BOARD_IDX_BLACK + KNIGHT] = A3;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            0);
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenBlackKnightHanging_ExpectPawnCapture) {
  position_[BOARD_IDX_WHITE] = C5;
  position_[BOARD_IDX_WHITE + PAWN] = C5;
  position_[BOARD_IDX_BLACK] = B6 | C6;
  position_[BOARD_IDX_BLACK + KNIGHT] = B6 | C6;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  const Bitmove& returned_move = *move_list_.begin();
  CheckMove(returned_move, C5, B6, PAWN, KNIGHT, NO_PROMOTION,
            MOVE_VALUE_TYPE_CAPTURE);
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenBlackRookHanging_ExpectPawnCapture) {
  position_[BOARD_IDX_WHITE] = D3;
  position_[BOARD_IDX_WHITE + PAWN] = D3;
  position_[BOARD_IDX_BLACK] = D4 | E4;
  position_[BOARD_IDX_BLACK + ROOK] = D4 | E4;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  const Bitmove& returned_move = *move_list_.begin();
  CheckMove(returned_move, D3, E4, PAWN, ROOK, NO_PROMOTION,
            MOVE_VALUE_TYPE_CAPTURE);
}

TEST_F(WhitePawnMoveGenerationFixture, GivenEnPassent1_ExpectCapture) {
  position_[BOARD_IDX_WHITE] = E5;
  position_[BOARD_IDX_WHITE + PAWN] = E5;
  position_[BOARD_IDX_BLACK] = E6 | F5;
  position_[BOARD_IDX_BLACK + PAWN] = E6 | F5;
  position_[BOARD_IDX_EXTRAS] |= static_cast<Bitboard>(tzcnt(F6))
                                 << BOARD_SHIFT_EN_PASSENT;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  const Bitmove& returned_move = *move_list_.begin();
  CheckMove(returned_move, E5, F6, PAWN, PAWN, NO_PROMOTION,
            MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
}

TEST_F(WhitePawnMoveGenerationFixture, GivenEnPassent2_ExpectCapture) {
  position_[BOARD_IDX_WHITE] = E5;
  position_[BOARD_IDX_WHITE + PAWN] = E5;
  position_[BOARD_IDX_BLACK] = E6 | D5;
  position_[BOARD_IDX_BLACK + PAWN] = E6 | D5;
  position_[BOARD_IDX_EXTRAS] |= static_cast<Bitboard>(tzcnt(D6))
                                 << BOARD_SHIFT_EN_PASSENT;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  const Bitmove& returned_move = *move_list_.begin();
  CheckMove(returned_move, E5, D6, PAWN, PAWN, NO_PROMOTION,
            MOVE_VALUE_TYPE_EN_PASSENT_CAPTURE);
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenPawnPushToPromotionRank_ExpectPromotion) {
  position_[BOARD_IDX_WHITE] = E7;
  position_[BOARD_IDX_WHITE + PAWN] = E7;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  ASSERT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            4);
  std::array<Bitmove, 4> returned_moves{};
  std::copy_n(move_list_.begin(), 4, returned_moves.begin());

  const Bitmove base_move =
      ComposeMove(tzcnt(E7), tzcnt(E8), PAWN, NO_CAPTURE, NO_PROMOTION,
                  MOVE_VALUE_TYPE_PROMOTION);
  const Bitmove queen_promotion = base_move | (QUEEN << MOVE_SHIFT_PROMOTION);
  const Bitmove rook_promotion = base_move | (ROOK << MOVE_SHIFT_PROMOTION);
  const Bitmove knight_promotion = base_move | (KNIGHT << MOVE_SHIFT_PROMOTION);
  const Bitmove bishop_promotion = base_move | (BISHOP << MOVE_SHIFT_PROMOTION);
  EXPECT_THAT(returned_moves, ::testing::UnorderedElementsAre(
                                  queen_promotion, rook_promotion,
                                  knight_promotion, bishop_promotion));
}

TEST_F(WhitePawnMoveGenerationFixture,
       GivenPawnCaptureOntoPromotionRank_ExpectPromotion) {
  position_[BOARD_IDX_WHITE] = D7;
  position_[BOARD_IDX_WHITE + PAWN] = D7;
  position_[BOARD_IDX_BLACK] = C8 | D8 | E8;
  position_[BOARD_IDX_BLACK + BISHOP] = C8;
  position_[BOARD_IDX_BLACK + KING] = D8;
  position_[BOARD_IDX_BLACK + KNIGHT] = E8;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  ASSERT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            8);
  std::array<Bitmove, 8> returned_moves{};
  std::copy_n(move_list_.begin(), 8, returned_moves.begin());

  const Bitmove base_move_left =
      ComposeMove(tzcnt(D7), tzcnt(C8), PAWN, BISHOP, NO_PROMOTION,
                  MOVE_VALUE_TYPE_PROMOTION);
  const Bitmove queen_promotion_capture_left =
      base_move_left | (QUEEN << MOVE_SHIFT_PROMOTION);
  const Bitmove rook_promotion_capture_left =
      base_move_left | (ROOK << MOVE_SHIFT_PROMOTION);
  const Bitmove knight_promotion_capture_left =
      base_move_left | (KNIGHT << MOVE_SHIFT_PROMOTION);
  const Bitmove bishop_promotion_capture_left =
      base_move_left | (BISHOP << MOVE_SHIFT_PROMOTION);
  const Bitmove base_move_right =
      ComposeMove(tzcnt(D7), tzcnt(E8), PAWN, KNIGHT, NO_PROMOTION,
                  MOVE_VALUE_TYPE_PROMOTION);
  const Bitmove queen_promotion_capture_right =
      base_move_right | (QUEEN << MOVE_SHIFT_PROMOTION);
  const Bitmove rook_promotion_capture_right =
      base_move_right | (ROOK << MOVE_SHIFT_PROMOTION);
  const Bitmove knight_promotion_capture_right =
      base_move_right | (KNIGHT << MOVE_SHIFT_PROMOTION);
  const Bitmove bishop_promotion_capture_right =
      base_move_right | (BISHOP << MOVE_SHIFT_PROMOTION);
  EXPECT_THAT(
      returned_moves,
      ::testing::UnorderedElementsAre(
          queen_promotion_capture_left, rook_promotion_capture_left,
          knight_promotion_capture_left, bishop_promotion_capture_left,
          queen_promotion_capture_right, rook_promotion_capture_right,
          knight_promotion_capture_right, bishop_promotion_capture_right));
}

class BlackPawnMoveGenerationFixture : public ::testing::Test {
 public:
  void SetUp() override {
    position_[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING;  // black to move
  }

  PositionWithBitboards position_{};
  MoveList move_list_{};
};

TEST_F(BlackPawnMoveGenerationFixture,
       GivenFreeSquareInfrontAndNotOnStartingRow_ExpectSinglePawnPush) {
  position_[BOARD_IDX_BLACK] = F5;
  position_[BOARD_IDX_BLACK + PAWN] = F5;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());
  const Bitmove& returned_move = *move_list_.begin();

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  CheckMove(returned_move, F5, F4, PAWN, NO_CAPTURE, NO_PROMOTION,
            MOVE_VALUE_TYPE_PAWN_PUSH);
}

TEST_F(BlackPawnMoveGenerationFixture, GivenNoFreeSquareInfront_ExpectNoMove) {
  position_[BOARD_IDX_BLACK] = F6;
  position_[BOARD_IDX_BLACK + PAWN] = F6;
  position_[BOARD_IDX_WHITE] = F5;
  position_[BOARD_IDX_WHITE + PAWN] = F5;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            0);
}

TEST_F(BlackPawnMoveGenerationFixture,
       GivenOnStartingRowWithTwoFreeSquaresInfront_ExpectDoublePawnPush) {
  position_[BOARD_IDX_BLACK] = G7;
  position_[BOARD_IDX_BLACK + PAWN] = G7;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            2);
  std::array<Bitmove, 2> returned_moves{};
  std::copy_n(move_list_.begin(), 2, returned_moves.begin());

  EXPECT_THAT(returned_moves,
              ::testing::Contains(
                  ComposeMove(tzcnt(G7), tzcnt(G5), PAWN, NO_CAPTURE,
                              NO_PROMOTION, MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH)));
}

}  // namespace
}  // namespace Chess
