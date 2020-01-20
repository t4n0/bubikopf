#include "search/test/minimax_abstract_examples.h"

#include <gtest/gtest.h>

namespace AlphaBeta {
namespace {

struct MinimaxTest_FixtureParameters {
  std::function<Node<float>()> tree_generator;
  std::vector<float> expected_order_of_evaluation;
  float expected_overall_evaluation;
};

class MinimaxTest_Fixture
    : public ::testing::TestWithParam<MinimaxTest_FixtureParameters> {
 public:
  void SetUp() { order_of_evaluation.resize(0); }
  void TearDown() {}
};

TEST_P(MinimaxTest_Fixture, GivenTree_ExpectEvaluationOrderAndFinalEvaluation) {
  // Setup
  Node<float> unit = GetParam().tree_generator();

  // Call
  Evaluation state_evaluation =
      minimax<float>(unit, 3, Player::max, MIN_EVAL, MAX_EVAL);
  float overall_evaluation{std::get<float>(state_evaluation)};

  // Expect
  EXPECT_FLOAT_EQ(overall_evaluation, GetParam().expected_overall_evaluation);
  EXPECT_EQ(GetParam().expected_order_of_evaluation.size(),
            order_of_evaluation.size());
  for (std::size_t idx{0}; idx < order_of_evaluation.size(); ++idx) {
    EXPECT_FLOAT_EQ(GetParam().expected_order_of_evaluation.at(idx),
                    order_of_evaluation.at(idx));
  }
}

MinimaxTest_FixtureParameters NEGATIVE_TREE_EXAMPLE{
    generate_negative_tree,
    {-1.0F, -2.0F, -3.0F, -4.0F, -5.0F,
     -6.0F},  // nodes -7 and -8 will be pruned
    -3.0F};

MinimaxTest_FixtureParameters POSITIVE_TREE_EXAMPLE{
    generate_positive_tree,
    {1.0F, 2.0F, 3.0F, 5.0F, 6.0F, 7.0F},  // nodes 4 and 8 will be pruned
    6.0F};

MinimaxTest_FixtureParameters TYPICAL_TREE_EXAMPLE{
    generate_typical_tree, {-1.0F, 3.0F, 5.0F, -6.0F, -4.0F}, 3.0F};

INSTANTIATE_TEST_SUITE_P(BasicExamples, MinimaxTest_Fixture,
                         ::testing::ValuesIn({NEGATIVE_TREE_EXAMPLE,
                                              POSITIVE_TREE_EXAMPLE,
                                              TYPICAL_TREE_EXAMPLE}));

}  // namespace
}  // namespace AlphaBeta
