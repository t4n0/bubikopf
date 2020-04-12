#include "search/minimax.h"
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

  const auto t1 = std::chrono::steady_clock::now();
  populate(*node_, depth);
  const auto t2 = std::chrono::steady_clock::now();
  minimax(*node_, depth, MIN_EVAL, MAX_EVAL);
  const auto t3 = std::chrono::steady_clock::now();

  const auto duration_populate =
      std::chrono::duration<double, std::milli>(t2 - t1);
  const auto duration_minimax =
      std::chrono::duration<double, std::milli>(t3 - t2);
  const int number_of_nodes = CountNodes(*node_);
  const std::chrono::duration<double, std::micro> duration_populate_per_node =
      duration_populate / number_of_nodes;
  const std::chrono::duration<double, std::micro> duration_minimax_per_node =
      duration_minimax / number_of_nodes;
  std::cout << "Populating with depth " << depth << " took "
            << duration_populate.count() << " millisec\n";
  std::cout << "Minimax with depth " << depth << " took "
            << duration_minimax.count() << " millisec\n";
  std::cout << "Per node: " << duration_populate_per_node.count()
            << " microsec populate and " << duration_minimax_per_node.count()
            << " microsec minimax\n";

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

}  // namespace
}  // namespace Chess
