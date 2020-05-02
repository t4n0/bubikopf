#include "search/minimax.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess {
namespace {

TEST(MinimaxBitBoardTest,
     GivenDepth3_ExpectDebuggingIdsOf6LastVisitedMovesInMoveList) {
  // Setup
  PositionWithBitboards position{};
  move_list_t move_list{};
  const int DEPTH{3};

  // Call
  minimax<SearchAllBranchesWithoutPruning, GenerateTwoMovesWithUniqueDebugId,
          EvaluteToZero>(DEPTH, position, move_list.begin());

  // Expect
  const std::array<move_t, 6> expexted_debugging_ids{
      1, 2, 9, 10, 13, 14};  // worked out by hand
  for (std::size_t idx{0}; idx < expexted_debugging_ids.size(); idx++) {
    EXPECT_EQ(expexted_debugging_ids.at(idx), move_list.at(idx));
  }
}

}  // namespace
}  // namespace Chess
