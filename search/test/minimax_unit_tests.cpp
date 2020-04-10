#include "search/minimax.h"

#include "search/test/evaluate_mock.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

static int GLOBAL_NODE_IDENTIFIER{0};
std::unique_ptr<Node> MakeChildWithUniqueIdHiddenInPliesVariable(
    const Node& node) {
  std::unique_ptr<Node> child = std::make_unique<Node>(State{});
  child->state_.static_plies_ = ++GLOBAL_NODE_IDENTIFIER;
  child->state_.plies_ = node.state_.plies_ + 1;
  return child;
}

// produces the following tree:                                    |
//                                                                 |
//                               0                                 |
//                              / \                                |
//                             /   \                               |
//                            /     \                              |
//                           /       \                             |
//                          /         \                            |
//                         /           \                           |
//                        /             \                          |
//                       /               \                         |
//                      /                 \                        |
//                     1                   2                       |
//                    / \                 / \                      |
//                   /   \               /   \                     |
//                  /     \             /     \                    |
//                 /       \           /       \                   |
//                3         4         9        10                  |
//               / \       / \       / \       / \                 |
//              /   \     /   \     /   \     /   \                |
//             5     6   7     8   11   12   13   14               |
//                                                                 |
// depicted number represents the id hidden in the plies variable  |
//                                                                 |
void PopulateWithTwoChildren(Node& node, const int depth) {
  if (depth) {
    node.children_.push_back(MakeChildWithUniqueIdHiddenInPliesVariable(node));
    node.children_.push_back(MakeChildWithUniqueIdHiddenInPliesVariable(node));

    for (std::size_t idx{}; idx < node.children_.size(); idx++) {
      PopulateWithTwoChildren(*node.children_.at(idx), depth - 1);
    }
  }
};

TEST(PopulateWithTwoChildrenTest, GivenDepth3_Expect15Nodes) {
  // Setup
  Node root_node{State{}};
  const int depth{2};

  // Call
  PopulateWithTwoChildren(root_node, depth);

  // Expect
  const int exected_number_of_nodes{7};
  const int returned_number_of_nodes{CountNodes(root_node)};
  EXPECT_EQ(exected_number_of_nodes, returned_number_of_nodes);
}

struct AllNodesEvaluateToSameValue {
  static float GetNodeValue(int /*unused*/) { return arbitrary_value; }
  static constexpr bool is_mock{true};
  static constexpr float arbitrary_value{42.0F};
};

struct MinimaxTestParameters {
  uint8_t search_depth;
  uint8_t tree_depth;
  int expected_number_of_evaluated_nodes;
};

struct MinimaxTest_Fixture
    : public ::testing::TestWithParam<MinimaxTestParameters> {
  void SetUp() override {
    ORDER_OF_NODE_EVALUATION.resize(0);
    GLOBAL_NODE_IDENTIFIER = 0;
  }
  void TearDown() override {}
};

TEST_P(MinimaxTest_Fixture, GivenDepths_ExpectCutOfAtSmallerDepth) {
  // Setup
  Node root_node{State{}};
  PopulateWithTwoChildren(root_node, GetParam().tree_depth);

  // Call
  minimax<AllNodesEvaluateToSameValue>(
      root_node, GetParam().search_depth,
      AllNodesEvaluateToSameValue::arbitrary_value,
      AllNodesEvaluateToSameValue::arbitrary_value);

  // Expect
  const std::size_t number_of_evaluated_nodes{ORDER_OF_NODE_EVALUATION.size()};
  EXPECT_EQ(number_of_evaluated_nodes,
            GetParam().expected_number_of_evaluated_nodes);
}

const MinimaxTestParameters SEARCH_DEPTH_GREATER_TREE_DEPTH{7, 3, 8};
const MinimaxTestParameters TREE_DEPTH_GREATER_SEARCH_DEPTH{2, 7, 4};
const std::vector<MinimaxTestParameters> SAMPLES{
    TREE_DEPTH_GREATER_SEARCH_DEPTH, SEARCH_DEPTH_GREATER_TREE_DEPTH};

INSTANTIATE_TEST_SUITE_P(TwoPermutations, MinimaxTest_Fixture,
                         ::testing::ValuesIn(SAMPLES));

template <typename T>
struct MinimaxTest_TypedFixture : public ::testing::Test {
  void SetUp() override {
    GLOBAL_NODE_IDENTIFIER = 0;
    ORDER_OF_NODE_EVALUATION.resize(0);
    PopulateWithTwoChildren(node_, depth_);
  }
  void TearDown() override {}

  Node node_{State{}};
  const int depth_{3};
};

struct LeafNodesEvaluateToDecreasingValues {
  static float GetNodeValue(int identifier) {
    const std::map<int, float> map_id_value{
        {5, -1.0F},  {6, -2.0F},  {7, -3.0F},  {8, -4.0F},
        {11, -5.0F}, {12, -6.0F}, {13, -7.0F}, {14, -8.0F}};
    return map_id_value.at(identifier);
  }
  static std::vector<int> GetExpectedEvaluationOrder() {
    return {5, 6, 7, 8, 11, 12};  // nodes 13 and 14 will be pruned
  }
  static float GetExpectedFinalEvaluation() { return -3.0F; }
  static constexpr bool is_mock{true};
};

struct LeafNodesEvaluateToIncreasingValues {
  static float GetNodeValue(int identifier) {
    const std::map<int, float> map_id_value{{5, 1.0F},  {6, 2.0F},  {7, 3.0F},
                                            {8, 4.0F},  {11, 5.0F}, {12, 6.0F},
                                            {13, 7.0F}, {14, 8.0F}};
    return map_id_value.at(identifier);
  }
  static std::vector<int> GetExpectedEvaluationOrder() {
    return {5, 6, 7, 11, 12, 13};  // nodes 8 and 14 will be pruned
  }
  static float GetExpectedFinalEvaluation() { return 6.0F; }
  static constexpr bool is_mock{true};
};

struct LeafNodesEvaluateToTypicalValues {
  static float GetNodeValue(int identifier) {
    const std::map<int, float> map_id_value{
        {5, -1.0F},  {6, 3.0F},   {7, 5.0F},   {8, 42.0F},
        {11, -4.0F}, {12, -6.0F}, {13, 43.0F}, {14, 44.0F}};
    return map_id_value.at(identifier);
  }
  static std::vector<int> GetExpectedEvaluationOrder() {
    return {5, 6, 7, 11, 12};  // nodes 13 and 14 will be pruned
  }
  static float GetExpectedFinalEvaluation() { return 3.0F; }
  static constexpr bool is_mock{true};
};

using EvaluateMocks = ::testing::Types<LeafNodesEvaluateToDecreasingValues,
                                       LeafNodesEvaluateToIncreasingValues,
                                       LeafNodesEvaluateToTypicalValues>;
TYPED_TEST_SUITE(MinimaxTest_TypedFixture, EvaluateMocks);

TYPED_TEST(
    MinimaxTest_TypedFixture,
    GivenInjectedNodeEvaluation_ExpectEvaluationOrderAndFinalEvaluation) {
  // Setup
  using EvaluateMock = TypeParam;

  // Call
  const Evaluation returned_evaluation = minimax<EvaluateMock>(
      TestFixture::node_, TestFixture::depth_, MIN_EVAL, MAX_EVAL);

  // Expect
  EXPECT_FLOAT_EQ(std::get<float>(returned_evaluation),
                  TypeParam::GetExpectedFinalEvaluation());
  EXPECT_EQ(ORDER_OF_NODE_EVALUATION, TypeParam::GetExpectedEvaluationOrder());
}

}  // namespace
}  // namespace Chess
