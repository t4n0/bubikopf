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

}  // namespace
}  // namespace Chess
