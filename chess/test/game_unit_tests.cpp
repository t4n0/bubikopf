#include "chess/game.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess {
namespace {

TEST(SetUpEmptyBoard, ExpectBoardIsFilledWithEmptySquares) {
  const State state{SetUpEmptyBoard()};
  for (const auto& square : state.board_.squares_) {
    EXPECT_TRUE(square->IsEmpty());
  }
}

TEST(SetUpBoard, ExpextStandardSetup) {
  const State state{SetUpBoard()};
  std::cout << state.board_;
  // check manually
}

}  // namespace
}  // namespace Chess
