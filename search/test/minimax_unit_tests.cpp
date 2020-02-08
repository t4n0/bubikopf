#include "search/minimax.h"

#include "board/utilities.h"
#include "search/test/evaluate_mock.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

static int GLOBAL_NODE_IDENTIFIER{0};
std::unique_ptr<Node> MakeChildWithUniqueIdHiddenInPliesVariable() {
  std::unique_ptr<Node> child = std::make_unique<Node>(SetUpEmptyBoard());
  child->state_.plies_ = ++GLOBAL_NODE_IDENTIFIER;
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
    node.children_.push_back(MakeChildWithUniqueIdHiddenInPliesVariable());
    node.children_.push_back(MakeChildWithUniqueIdHiddenInPliesVariable());

    for (std::size_t idx{}; idx < node.children_.size(); idx++) {
      PopulateWithTwoChildren(*node.children_.at(idx), depth - 1);
    }
  }
};

template <typename T>
struct MinimaxTest : public ::testing::Test {
  void SetUp() override {
    GLOBAL_NODE_IDENTIFIER = 0;
    ORDER_OF_NODE_EVALUATION.resize(0);
    PopulateWithTwoChildren(node_, depth_);
  }
  void TearDown() override {}

  Node node_{SetUpEmptyBoard()};
  const int depth_{3};
};

struct LeafNodesEvaluateToDecreasingValues {
  static std::map<int, float> GetNodeValueMap() {
    return {{5, -1.0F},  {6, -2.0F},  {7, -3.0F},  {8, -4.0F},
            {11, -5.0F}, {12, -6.0F}, {13, -7.0F}, {14, -8.0F}};
  }
  static std::vector<int> GetExpectedEvaluationOrder() {
    return {5, 6, 7, 8, 11, 12};  // nodes 13 and 14 will be pruned
  }
  static float GetExpectedFinalEvaluation() { return -3.0F; }
  static constexpr bool is_mock{true};
};

struct LeafNodesEvaluateToIncreasingValues {
  static std::map<int, float> GetNodeValueMap() {
    return {{5, 1.0F},  {6, 2.0F},  {7, 3.0F},  {8, 4.0F},
            {11, 5.0F}, {12, 6.0F}, {13, 7.0F}, {14, 8.0F}};
  }
  static std::vector<int> GetExpectedEvaluationOrder() {
    return {5, 6, 7, 11, 12, 13};  // nodes 8 and 14 will be pruned
  }
  static float GetExpectedFinalEvaluation() { return 6.0F; }
  static constexpr bool is_mock{true};
};

struct LeafNodesEvaluateToTypicalValues {
  static std::map<int, float> GetNodeValueMap() {
    return {{5, -1.0F},  {6, 3.0F},   {7, 5.0F},   {8, 42.0F},
            {11, -4.0F}, {12, -6.0F}, {13, 43.0F}, {14, 44.0F}};
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
TYPED_TEST_SUITE(MinimaxTest, EvaluateMocks);

TYPED_TEST(
    MinimaxTest,
    GivenInjectedNodeEvaluation_ExpectEvaluationOrderAndFinalEvaluation) {
  // Setup
  using EvaluateMock = TypeParam;

  // Call
  const Evaluation returned_evaluation = minimax<EvaluateMock>(
      TestFixture::node_, TestFixture::depth_, Player::max, MIN_EVAL, MAX_EVAL);

  // Expect
  EXPECT_FLOAT_EQ(std::get<float>(returned_evaluation),
                  TypeParam::GetExpectedFinalEvaluation());
  EXPECT_EQ(ORDER_OF_NODE_EVALUATION, TypeParam::GetExpectedEvaluationOrder());
}

}  // namespace
}  // namespace Chess
