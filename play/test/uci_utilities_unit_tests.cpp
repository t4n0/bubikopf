#include "play/uci_utilities.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(SeparateEachMove, GivenEmptyString_ExpectZeroElements) {
  // Setup
  const std::string moves{};

  // Call
  const std::vector<std::string> returned_separated_moves{
      SeparateEachMove(moves)};

  // Expect
  EXPECT_TRUE(returned_separated_moves.empty());
}

TEST(SeparateEachMove, GivenOneMove_ExpectThatMoveOnly) {
  // Setup
  const std::string movess{"e2e4"};

  // Call
  const std::vector<std::string> returned_separated_moves{
      SeparateEachMove(movess)};

  // Expect
  const std::vector<std::string> expected_separated_moves{"e2e4"};
  EXPECT_TRUE(returned_separated_moves.size() == 1);
  EXPECT_EQ(returned_separated_moves, expected_separated_moves);
}

TEST(SeparateEachMove, GivenTwoMoves_ExpectTheseTwo) {
  // Setup
  const std::string moves{"e2e4 e7e5"};

  // Call
  const std::vector<std::string> returned_separated_moves{
      SeparateEachMove(moves)};

  // Expect
  const std::vector<std::string> expected_move{"e2e4", "e7e5"};
  EXPECT_TRUE(returned_separated_moves.size() == 2);
  EXPECT_EQ(returned_separated_moves, expected_move);
}

TEST(SeparateEachMove, GivenTwoBlanks_ExpectTheseTwo) {
  // Setup
  const std::string moves{"e2e4  e7e5"};  // to whitespaces between moves

  // Call
  const std::vector<std::string> returned_separated_moves{
      SeparateEachMove(moves)};

  // Expect
  const std::vector<std::string> expected_move{"e2e4", "e7e5"};
  EXPECT_TRUE(returned_separated_moves.size() == 2);
  EXPECT_EQ(returned_separated_moves, expected_move);
}

}  // namespace
}  // namespace Chess
