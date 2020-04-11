#include "search/populate.h"

#include "board/pieces.h"

#include <gtest/gtest.h>

#include <chrono>

namespace Chess {
namespace {

class Populate_Fixture : public testing::Test {
 public:
  NodePtr node_{std::make_unique<Node>()};
};

TEST_F(Populate_Fixture, GivenDepth1_Expect20children) {
  const int depth{1};
  const auto t1 = std::chrono::high_resolution_clock::now();
  populate(*node_, depth);
  const auto t2 = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

  std::cout << "Populating with depth " << depth << " took " << duration
            << " microsec\n";

  EXPECT_TRUE(node_->children_.size() == 20);
}

TEST(PopulateSmokeTest,
     GivenKingsInOppositeCornersAndSearchDepth4_ExpectCorrectNumberOfChildren) {
  // Setup
  const int depth = 3;
  NodePtr node_{std::make_unique<Node>(Position{})};
  node_->position_.board_.Set(0, King::OfSide(Player::max));
  node_->position_.board_.Set(63, King::OfSide(Player::min));
  populate(*node_, depth);

  // Call
  const int returned_number_of_children{CountNodes(*node_)};

  // Expect
  const int expected_number_of_moves = 67;  // worked out by hand
  EXPECT_EQ(returned_number_of_children, expected_number_of_moves);
}

TEST(Node, MemoryFootprint) {
  NodePtr node{std::make_unique<Node>(Position{})};
  populate(*node, 1);

  EXPECT_EQ(sizeof(Node), 552);
  // consisting of
  EXPECT_EQ(sizeof(Position), 528);
  EXPECT_EQ(sizeof(std::vector<std::unique_ptr<Node>>), 24);
}

}  // namespace
}  // namespace Chess
