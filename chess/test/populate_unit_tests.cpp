#include <chess/populate.h>

#include <gtest/gtest.h>

namespace Chess {
namespace {

void PrintNode(const Node& node) {
  std::cout << "Node:\n";
  std::cout << node.state_.board_;

  if (node.children_.size()) {
    std::cout << "Children:\n";
  }
  for (const std::unique_ptr<Node>& child : node.children_) {
    std::cout << child->state_.board_;
  }
}

class FindLegalMovesTest : public testing::Test {
 public:
  void SetUp() override {}
  void TearDown() override { PrintNode(node_); }

  Node node_{State{}};
};

TEST_F(FindLegalMovesTest, WithEmptyBoard_ExpectNoMoves) {
  node_.children_ = detail::find_legal_moves(node_);
  const std::size_t expected_children{0};

  EXPECT_EQ(expected_children, node_.children_.size());
}

}  // namespace
}  // namespace Chess
