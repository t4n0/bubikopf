#include "board/utilities.h"
#include "search/evaluate.h"
#include "search/populate.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Evaluate_Fixture, GivenScholarsMate_ExpectCorrectDefence) {
  Node node{SetUpBoard()};
  node.state_.board_.SwapSquares(1, 18);
  node.state_.board_.SwapSquares(0, 1);
  node.state_.board_.SwapSquares(34, 61);
  node.state_.board_.SwapSquares(36, 52);
  node.state_.board_.SwapSquares(45, 59);
  node.state_.turn_ = Player::min;

  populate(node, 5);
}

}  // namespace
}  // namespace Chess
