#include "board/utilities.h"
#include "search/evaluate.h"
#include "search/minimax.h"
#include "search/populate.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

struct Puzzle_Fixture : public ::testing::Test {
  Node root_node_{SetUpEmptyBoard()};
  Board& root_board_{root_node_.state_.board_};
  const SquareBehaviourPool& pool_{root_node_.state_.pool_};
};

TEST_F(Puzzle_Fixture, GivenMateInOne_ExpectWin) {
  // Setup
  root_board_.Set(ToIdx('a', 8), pool_.GetKing(Player::min));
  root_board_.Set(ToIdx('h', 1), pool_.GetKing(Player::max));
  root_board_.Set(ToIdx('b', 1), pool_.GetRook(Player::max));
  root_board_.Set(ToIdx('c', 2), pool_.GetQueen(Player::max));
  std::cout << root_node_.state_ << '\n';

  const int DEPTH = 3;
  populate(root_node_, DEPTH);
  std::cout << "Tree depth " << DEPTH << " generated " << CountNodes(root_node_)
            << " nodes\n";

  // Call
  const Evaluation evaluation = minimax(root_node_, DEPTH, MIN_EVAL, MAX_EVAL);
  std::cout << "Final evaluation " << evaluation << '\n';

  // Expect
  const Evaluation minimum_expected_evaluation{99.0F};
  EXPECT_GT(evaluation, minimum_expected_evaluation);
}

}  // namespace
}  // namespace Chess
