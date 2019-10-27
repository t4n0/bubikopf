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

class FindMovesTest : public testing::Test {
 public:
  void SetUp() override {}
  void TearDown() override { PrintNode(node_); }

  Node node_{State{}};
};

TEST_F(FindMovesTest, GivenEmptySquare_ExpectNoMoves) {
  const Piece piece{Piece::e};
  const Coordinate location{2, 2};
  node_.state_.board_.Set(location, piece);

  node_.children_ = detail::find_moves<piece>(node_, location);

  const std::size_t expected_children{0};
  EXPECT_EQ(expected_children, node_.children_.size());
}

TEST_F(FindMovesTest, GivenBlackKingOnly_Expect8Moves) {
  const Piece piece{Piece::k};
  const Coordinate location{2, 2};
  node_.state_.board_.Set(location, piece);

  node_.children_ = detail::find_moves<piece>(node_, location);

  const std::size_t expected_children{8};
  EXPECT_EQ(expected_children, node_.children_.size());
}

TEST_F(FindMovesTest, GivenWhiteKingOnBlacksMove_ExpectNoMoves) {
  const Piece piece{Piece::K};
  const Coordinate location{2, 2};
  node_.state_.board_.Set(location, piece);

  node_.children_ = detail::find_moves<piece>(node_, location);

  const std::size_t expected_children{0};

  EXPECT_EQ(expected_children, node_.children_.size());
}

TEST_F(FindMovesTest, GivenBlackKingOnlyInCorner_Expect3Moves) {
  const Piece piece{Piece::k};
  const Coordinate location{7, 7};
  node_.state_.board_.Set(location, piece);
  node_.children_ = detail::find_moves<piece>(node_, location);

  const std::size_t expected_children{3};

  EXPECT_EQ(expected_children, node_.children_.size());
}

TEST_F(FindMovesTest, GivenWhiteKingAdjacentWhitePawn_Expect7Moves) {
  node_.state_.turn_ = GameTree::Player::max;
  node_.state_.board_.Set({5, 5}, Piece::K);
  node_.state_.board_.Set({4, 5}, Piece::P);

  node_.children_ = detail::find_legal_moves(node_);

  const std::size_t expected_children{7};
  EXPECT_EQ(expected_children, node_.children_.size());
}

TEST_F(FindMovesTest, GivenWhiteKingAdjacentBlackPawn_Expect8Moves) {
  node_.state_.turn_ = GameTree::Player::max;
  node_.state_.board_.Set({5, 5}, Piece::K);
  node_.state_.board_.Set({4, 5}, Piece::p);

  node_.children_ = detail::find_legal_moves(node_);

  const std::size_t expected_children{8};
  EXPECT_EQ(expected_children, node_.children_.size());
}

}  // namespace
}  // namespace Chess
