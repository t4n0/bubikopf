#include "chess/evaluate.h"
#include "chess/pieces.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

using AlphaBeta::Player;

TEST(EvaluateTest, GivenSamples_ExpectAllHold) {
  const SquareBehaviour pool{};
  State state{pool};

  state.board_.Set({0, 0}, std::make_unique<Pawn>(Player::min));
  AlphaBeta::Evaluation returned_evaluation{AlphaBeta::evaluate(state)};
  EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

  state.board_.Set({0, 1}, std::make_unique<Pawn>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({1, 0}, std::make_unique<Knight>(Player::min));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({1, 1}, std::make_unique<Knight>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({3, 0}, std::make_unique<Bishop>(Player::min));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({3, 1}, std::make_unique<Bishop>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({4, 0}, std::make_unique<Rook>(Player::min));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

  state.board_.Set({4, 1}, std::make_unique<Rook>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({5, 0}, std::make_unique<Queen>(Player::min));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

  state.board_.Set({5, 1}, std::make_unique<Queen>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({6, 0}, std::make_unique<King>(Player::min));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

  state.board_.Set({6, 1}, std::make_unique<King>(Player::max));
  returned_evaluation = AlphaBeta::evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

}  // namespace
}  // namespace Chess
