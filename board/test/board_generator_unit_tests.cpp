#include "board/utilities.h"

#include <gtest/gtest.h>

#include <iostream>

namespace Chess {
namespace {

TEST(SetUpBoard, ExpextStandardSetup) {
  const State state{SetUpBoard()};
  std::cout << state.board_;
  // check manually
}

}  // namespace
}  // namespace Chess
