#include "hardware/trailing_zeros_count.h"

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(TrailingZeroCountTest, GivenAllZeros_Expect64) {
  constexpr std::uint64_t all_zeros{0};
  const int expected_bit{64};
  EXPECT_EQ(tzcnt(all_zeros), expected_bit);
}

}  // namespace
}  // namespace Chess
