#include "search/evaluate.h"

#include "board/utilities.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Evaluate, GivenSamples_ExpectAllHold) {
  State state{SetUpEmptyBoard()};
  state.board_.Set({0, 0}, state.pool_.GetPawn(Player::min));
  Evaluation returned_evaluation{evaluate(state)};
  EXPECT_EQ(std::get<float>(returned_evaluation), -1.0F);

  state.board_.Set({0, 1}, state.pool_.GetPawn(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({1, 0}, state.pool_.GetKnight(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({1, 1}, state.pool_.GetKnight(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({3, 0}, state.pool_.GetBishop(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -3.0F);

  state.board_.Set({3, 1}, state.pool_.GetBishop(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({4, 0}, state.pool_.GetRook(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -5.0F);

  state.board_.Set({4, 1}, state.pool_.GetRook(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({5, 0}, state.pool_.GetQueen(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -9.0F);

  state.board_.Set({5, 1}, state.pool_.GetQueen(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);

  state.board_.Set({6, 0}, state.pool_.GetKing(Player::min));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), -100.0F);

  state.board_.Set({6, 1}, state.pool_.GetKing(Player::max));
  returned_evaluation = evaluate(state);
  EXPECT_EQ(std::get<float>(returned_evaluation), 0.0F);
};

}  // namespace
}  // namespace Chess
