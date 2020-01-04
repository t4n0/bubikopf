#include "chess/pieces.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(Clone, ExpectEquality) {
  // Setup
  const std::unique_ptr<ISquare> original =
      std::make_unique<King>(AlphaBeta::Player::max);

  // Call
  const std::unique_ptr<ISquare> clone = original->clone();

  // Expect
  EXPECT_FLOAT_EQ(original->GetValue(), clone->GetValue());
  EXPECT_TRUE(clone->IsOfSide(AlphaBeta::Player::max));
}

}  // namespace
}  // namespace Chess
