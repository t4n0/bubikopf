#include "minimax_abstract_examples.h"

#include <gtest/gtest.h>

namespace GameTree {

namespace {

class MinimaxTest : public ::testing::Test {
 public:
  void SetUp() { order_of_evaluation.resize(0); }
  void TearDown() {}
};

TEST_F(MinimaxTest, NegativeTree_MustMeetExpectedEvaluationOrder) {
  Node<float> unit = generate_negative_tree();

  Evaluation state_evaluation =
      minimax<float>(unit, 3, Player::max, MIN_EVAL, MAX_EVAL);
  float overall_evaluation{std::get<float>(state_evaluation)};

  const std::vector<float> expected_order_of_evaluation{
      -1.0F, -2.0F, -3.0F,
      -4.0F, -5.0F, -6.0F};  // nodes -7 and -8 will be pruned
  const float expected_overall_evaluation{-3.0F};

  EXPECT_FLOAT_EQ(overall_evaluation, expected_overall_evaluation);
  EXPECT_EQ(expected_order_of_evaluation.size(), order_of_evaluation.size());

  for (std::size_t idx{0}; idx < order_of_evaluation.size(); ++idx) {
    EXPECT_FLOAT_EQ(expected_order_of_evaluation.at(idx),
                    order_of_evaluation.at(idx));
  }
}

TEST_F(MinimaxTest, PositiveTree_MustMeetExpectedEvaluationOrder) {
  Node<float> unit = generate_positive_tree();

  Evaluation state_evaluation =
      minimax<float>(unit, 3, Player::max, MIN_EVAL, MAX_EVAL);
  float overall_evaluation{std::get<float>(state_evaluation)};

  const std::vector<float> expected_order_of_evaluation{
      1.0F, 2.0F, 3.0F, 5.0F, 6.0F, 7.0F};  // nodes 4 and 8 will be pruned
  const float expected_overall_evaluation{6.0F};

  EXPECT_FLOAT_EQ(overall_evaluation, expected_overall_evaluation);
  EXPECT_EQ(expected_order_of_evaluation.size(), order_of_evaluation.size());

  for (std::size_t idx{0}; idx < order_of_evaluation.size(); ++idx) {
    EXPECT_FLOAT_EQ(expected_order_of_evaluation.at(idx),
                    order_of_evaluation.at(idx));
  }
}

TEST_F(MinimaxTest, TypicalTree_MustMeetExpectedEvaluationOrder) {
  Node<float> unit = generate_typical_tree();

  Evaluation state_evaluation =
      minimax<float>(unit, 3, Player::max, MIN_EVAL, MAX_EVAL);
  float overall_evaluation{std::get<float>(state_evaluation)};

  const std::vector<float> expected_order_of_evaluation{-1.0F, 3.0F, 5.0F,
                                                        -6.0F, -4.0F};
  const float expected_overall_evaluation{3.0F};

  EXPECT_FLOAT_EQ(overall_evaluation, expected_overall_evaluation);
  EXPECT_EQ(expected_order_of_evaluation.size(), order_of_evaluation.size());

  for (std::size_t idx{0}; idx < order_of_evaluation.size(); ++idx) {
    EXPECT_FLOAT_EQ(expected_order_of_evaluation.at(idx),
                    order_of_evaluation.at(idx));
  }
}

}  // namespace

}  // namespace GameTree
