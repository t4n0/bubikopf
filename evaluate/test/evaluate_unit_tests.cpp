#include "evaluate/evaluate.h"

#include "board/piece.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Evaluate, GivenSamples_ExpectAllHold) {
  State state{};
  state.board_.Set({0, 0}, Pawn::OfSide(Player::min));
  Evaluation returned_evaluation{evaluate(state)};
  EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

  state.board_.Set({0, 1}, Pawn::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({1, 0}, Knight::OfSide(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({1, 1}, Knight::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({3, 0}, Bishop::OfSide(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({3, 1}, Bishop::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({4, 0}, Rook::OfSide(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

  state.board_.Set({4, 1}, Rook::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({5, 0}, Queen::OfSide(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

  state.board_.Set({5, 1}, Queen::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({6, 0}, King::OfSide(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

  state.board_.Set({6, 1}, King::OfSide(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

}  // namespace
}  // namespace Chess
