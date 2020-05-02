#include "evaluate/evaluate.h"

#include "board/pieces.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Evaluate, GivenSamples_ExpectAllHold) {
  Position position{};
  position.board_.Set({0, 0}, Pawn::OfSide(Player::min));
  Evaluation returned_evaluation{evaluate(position)};
  EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

  position.board_.Set({0, 1}, Pawn::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position.board_.Set({1, 0}, Knight::OfSide(Player::min));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  position.board_.Set({1, 1}, Knight::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position.board_.Set({3, 0}, Bishop::OfSide(Player::min));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  position.board_.Set({3, 1}, Bishop::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position.board_.Set({4, 0}, Rook::OfSide(Player::min));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

  position.board_.Set({4, 1}, Rook::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position.board_.Set({5, 0}, Queen::OfSide(Player::min));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

  position.board_.Set({5, 1}, Queen::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position.board_.Set({6, 0}, King::OfSide(Player::min));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

  position.board_.Set({6, 1}, King::OfSide(Player::max));
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

TEST(EvaluteMaterial, GivenSamples_ExpectAllHold) {
  PositionWithBitboards position{};

  position[BOARD_IDX_BLACK + PAWN] = 1;
  Evaluation returned_evaluation{evaluate(position)};
  EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

  position[BOARD_IDX_WHITE + PAWN] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position[BOARD_IDX_BLACK + KNIGHT] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  position[BOARD_IDX_WHITE + KNIGHT] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position[BOARD_IDX_BLACK + BISHOP] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  position[BOARD_IDX_WHITE + BISHOP] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position[BOARD_IDX_BLACK + ROOK] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

  position[BOARD_IDX_WHITE + ROOK] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position[BOARD_IDX_BLACK + QUEEN] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

  position[BOARD_IDX_WHITE + QUEEN] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  position[BOARD_IDX_BLACK + KING] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

  position[BOARD_IDX_WHITE + KING] = 1;
  returned_evaluation = evaluate(position);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

}  // namespace
}  // namespace Chess
