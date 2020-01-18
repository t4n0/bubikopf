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

}  // namespace
}  // namespace Chess
