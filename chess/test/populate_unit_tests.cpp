#include "chess/populate.h"

#include "chess/game.h"

#include <gtest/gtest.h>

#include <chrono>

namespace Chess {
namespace {

class Populate_Fixture : public testing::Test {
 public:
  NodePtr node_{std::make_unique<Node>(SetUpBoard())};
};

TEST_F(Populate_Fixture, GivenDepth1_Expect20children) {
  const int depth{1};
  const auto t1 = std::chrono::high_resolution_clock::now();
  AlphaBeta::populate(*node_, depth);
  const auto t2 = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

  std::cout << "Populating with depth " << depth << " took " << duration
            << " microsec\n";

  EXPECT_TRUE(node_->children_.size() == 20);
}

TEST(CountChildren,
     GivenKingsInOppositeCornersAndSearchDepth4_ExpectCorrectNumberOfChildren) {
  // Setup
  const int depth = 3;
  NodePtr node_{std::make_unique<Node>(SetUpEmptyBoard())};
  node_->state_.board_.Set(0,
                           node_->state_.pool_.GetKing(AlphaBeta::Player::max));
  node_->state_.board_.Set(63,
                           node_->state_.pool_.GetKing(AlphaBeta::Player::min));
  AlphaBeta::populate(*node_, depth);

  // Call
  const int returned_number_of_children{CountChildren(*node_)};

  // Expect
  const int expected_number_of_moves = 67;  // worked out by hand
  EXPECT_EQ(returned_number_of_children, expected_number_of_moves);
}

TEST(Node, MemoryFootprint) {
  NodePtr node{std::make_unique<Node>(SetUpEmptyBoard())};
  AlphaBeta::populate(*node, 1);

  EXPECT_EQ(sizeof(Node), 1080);
  // consisting of
  EXPECT_EQ(sizeof(State), 1056);
  EXPECT_EQ(sizeof(std::vector<std::unique_ptr<Node>>), 24);
}

}  // namespace
}  // namespace Chess
