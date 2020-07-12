#include "bitboard/move_generation.h"
#include "bitboard/squares.h"
#include "hardware/population_count.h"
#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

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

class MoveGeneration_Fixture : public ::testing::Test {
 public:
  void SetUp() override {
    position_[BOARD_IDX_EXTRAS] = BOARD_MASK_CASTLING | BOARD_MASK_WHITE_TURN;
  }

  PositionWithBitboards position_{};
  MoveList move_list_{};
};

TEST_F(
    MoveGeneration_Fixture,
    GivenWhitePawnWithFreeSquareInfrontAndNotOnStartingRow_ExpectSinglePawnPush) {
  position_[BOARD_IDX_WHITE] = F5;
  position_[BOARD_IDX_WHITE + PAWN] = F5;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());
  const Bitmove& returned_move = *move_list_.begin();

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            1);
  CheckMove(returned_move, F5, F6, PAWN, NO_PIECE, NO_PROMOTION,
            MOVE_VALUE_TYPE_PAWN_PUSH);
}

TEST_F(MoveGeneration_Fixture,
       GivenWhitePawnWithNoFreeSquareInfront_ExpectNoMove) {
  position_[BOARD_IDX_WHITE] = F6;
  position_[BOARD_IDX_WHITE + PAWN] = F6;
  position_[BOARD_IDX_BLACK] = F7;
  position_[BOARD_IDX_BLACK + PAWN] = F7;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            0);
}

TEST_F(
    MoveGeneration_Fixture,
    GivenWhitePawnOnStartingRowWithTwoFreeSquaresInfront_ExpectDoublePawnPush) {
  position_[BOARD_IDX_WHITE] = E2;
  position_[BOARD_IDX_WHITE + PAWN] = E2;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            2);
  for (const auto& move : move_list_) {
    if ((move & MOVE_MASK_TYPE) == MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH) {
      CheckMove(move, E2, E4, PAWN, NO_PIECE, NO_PROMOTION,
                MOVE_VALUE_TYPE_PAWN_DOUBLE_PUSH);
      break;
    }
  }
}

TEST_F(MoveGeneration_Fixture,
       GivenWhitePawnOnStartingRowWithNoFreeSquaresInfront_ExpectNoMove) {
  position_[BOARD_IDX_WHITE] = A2;
  position_[BOARD_IDX_WHITE + PAWN] = A2;
  position_[BOARD_IDX_BLACK] = A3;
  position_[BOARD_IDX_BLACK + KNIGHT] = A3;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            0);
}

TEST_F(MoveGeneration_Fixture, GivenBlackKnightHanging_ExpectPawnCapture) {
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

TEST_F(MoveGeneration_Fixture, GivenBlackRookHanging_ExpectPawnCapture) {
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

TEST_F(MoveGeneration_Fixture, GivenEnPassent1_ExpectCapture) {
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

TEST_F(MoveGeneration_Fixture, GivenEnPassent2_ExpectCapture) {
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

TEST_F(MoveGeneration_Fixture, GivenPawnPushToPromotionRank_ExpectPromotion) {
  position_[BOARD_IDX_WHITE] = E7;
  position_[BOARD_IDX_WHITE + PAWN] = E7;

  const MoveList::iterator returned_move_insertion_iterator =
      GenerateMoves(position_, move_list_.begin());

  EXPECT_EQ(std::distance(move_list_.begin(), returned_move_insertion_iterator),
            4);
  CheckMove(move_list_.at(0), E7, E8, PAWN, NO_PIECE, QUEEN,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(1), E7, E8, PAWN, NO_PIECE, ROOK,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(2), E7, E8, PAWN, NO_PIECE, KNIGHT,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(3), E7, E8, PAWN, NO_PIECE, BISHOP,
            MOVE_VALUE_TYPE_PROMOTION);
}

TEST_F(MoveGeneration_Fixture,
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
  CheckMove(move_list_.at(0), D7, C8, PAWN, BISHOP, QUEEN,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(1), D7, C8, PAWN, BISHOP, ROOK,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(2), D7, C8, PAWN, BISHOP, KNIGHT,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(3), D7, C8, PAWN, BISHOP, BISHOP,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(4), D7, E8, PAWN, KNIGHT, QUEEN,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(5), D7, E8, PAWN, KNIGHT, ROOK,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(6), D7, E8, PAWN, KNIGHT, KNIGHT,
            MOVE_VALUE_TYPE_PROMOTION);
  CheckMove(move_list_.at(7), D7, E8, PAWN, KNIGHT, BISHOP,
            MOVE_VALUE_TYPE_PROMOTION);
}

}  // namespace
}  // namespace Chess
