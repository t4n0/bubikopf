#include "chess/game.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(SetUpEmptyBoard, ExpectBoardIsFilledWithEmptySquares) {
  const State state{SetUpEmptyBoard()};
  for (const auto& square : state.board_.squares_) {
    EXPECT_TRUE(square->IsEmpty());
  }
}

}  // namespace
}  // namespace Chess
