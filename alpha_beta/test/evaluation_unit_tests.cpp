#include <alpha_beta/evaluation.h>

#include <gtest/gtest.h>

#include <iostream>

namespace GameTree {

namespace {

struct EvaluationComparisonOperatorParameters {
  Evaluation lhs{};
  Evaluation rhs{};
  bool expected_result{};
};

class EvaluationTest
    : public ::testing::TestWithParam<EvaluationComparisonOperatorParameters> {
};

TEST_P(EvaluationTest, ComparisonOperator) {
  EXPECT_EQ(GetParam().lhs < GetParam().rhs, GetParam().expected_result);
}

const std::vector<EvaluationComparisonOperatorParameters>
    comparison_permutations = {
        {Evaluation{Player::min}, Evaluation{Player::max}, true},
        {Evaluation{Player::min}, Evaluation{Player::min}, false},
        {Evaluation{Player::max}, Evaluation{Player::min}, false},
        {Evaluation{Player::max}, Evaluation{Player::max}, false},
        {Evaluation{Player::min}, Evaluation{int8_t{42}}, true},
        {Evaluation{Player::min}, Evaluation{0.42F}, true},
        {Evaluation{Player::max}, Evaluation{int8_t{42}}, false},
        {Evaluation{Player::max}, Evaluation{0.42F}, false},

        {Evaluation{int8_t{42}}, Evaluation{Player::max}, true},
        {Evaluation{int8_t{42}}, Evaluation{Player::min}, false},
        {Evaluation{int8_t{-42}}, Evaluation{int8_t{42}}, true},
        {Evaluation{int8_t{42}}, Evaluation{int8_t{-42}}, false},
        {Evaluation{int8_t{-42}}, Evaluation{0.42F}, true},
        {Evaluation{int8_t{42}}, Evaluation{-0.42F}, false},

        {Evaluation{0.42F}, Evaluation{Player::max}, true},
        {Evaluation{0.42F}, Evaluation{Player::min}, false},
        {Evaluation{0.42F}, Evaluation{int8_t{42}}, true},
        {Evaluation{0.42F}, Evaluation{int8_t{-42}}, false},
        {Evaluation{-0.42F}, Evaluation{0.42F}, true},
        {Evaluation{0.42F}, Evaluation{-0.42F}, false},

};

INSTANTIATE_TEST_CASE_P(AllPermutations, EvaluationTest,
                        ::testing::ValuesIn(comparison_permutations));

TEST(EvaluationTest, StreamOutputOperator_MustNotThrow) {
  const Evaluation player_type_max_example{Player::max};
  const Evaluation player_type_min_example{Player::min};
  const Evaluation int8_type_pos_example{int8_t(42)};
  const Evaluation int8_type_neg_example{int8_t(-42)};
  const Evaluation float_type_pos_example{0.42F};
  const Evaluation float_type_neg_example{-0.42F};

  EXPECT_NO_THROW(std::cout << float_type_pos_example << '\n');
  EXPECT_NO_THROW(std::cout << float_type_neg_example << '\n');
  EXPECT_NO_THROW(std::cout << int8_type_pos_example << '\n');
  EXPECT_NO_THROW(std::cout << int8_type_neg_example << '\n');
  EXPECT_NO_THROW(std::cout << player_type_max_example << '\n');
  EXPECT_NO_THROW(std::cout << player_type_min_example << '\n');
}

}  // namespace

}  // namespace GameTree
