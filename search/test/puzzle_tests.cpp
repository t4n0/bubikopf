#include "board/pieces.h"
#include "evaluate/evaluate.h"
#include "search/minimax.h"
#include "search/populate.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

struct Puzzle_Fixture : public ::testing::Test {
  Node root_node_{Position{}};
  Board& root_board_{root_node_.position_.board_};
};

TEST_F(Puzzle_Fixture, GivenMateInOneForWhite_ExpectWhiteWin) {
  // Setup
  root_board_.Set(ToIdx('a', 8), King::OfSide(Player::min));
  root_board_.Set(ToIdx('h', 1), King::OfSide(Player::max));
  root_board_.Set(ToIdx('b', 1), Rook::OfSide(Player::max));
  root_board_.Set(ToIdx('c', 2), Queen::OfSide(Player::max));
  root_node_.position_.plies_ = 4;
  std::cout << root_node_.position_ << '\n';

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

TEST_F(Puzzle_Fixture, GivenMateInOneForBlack_ExpectBlackWin) {
  // Setup
  root_board_.Set(ToIdx('a', 8), King::OfSide(Player::max));
  root_board_.Set(ToIdx('h', 1), King::OfSide(Player::min));
  root_board_.Set(ToIdx('b', 1), Rook::OfSide(Player::min));
  root_board_.Set(ToIdx('c', 2), Queen::OfSide(Player::min));
  root_node_.position_.plies_ = 1;  // make it blacks turn
  std::cout << root_node_.position_ << '\n';

  const int DEPTH = 3;
  populate(root_node_, DEPTH);
  std::cout << "Tree depth " << DEPTH << " generated " << CountNodes(root_node_)
            << " nodes\n";

  // Call
  const Evaluation evaluation = minimax(root_node_, DEPTH, MIN_EVAL, MAX_EVAL);
  std::cout << "Final evaluation " << evaluation << '\n';

  // Expect
  const Evaluation minimum_expected_evaluation{-99.0F};
  EXPECT_LT(evaluation, minimum_expected_evaluation);
}

}  // namespace
}  // namespace Chess
