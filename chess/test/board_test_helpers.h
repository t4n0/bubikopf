#ifndef CHESS_BOARD_TEST_HELPERS_H
#define CHESS_BOARD_TEST_HELPERS_H

#include <chess/state.h>

#include <gtest/gtest.h>

namespace Chess {
namespace detail {

inline bool BoardIsConsistent(const BoardContainer& board) {
  return std::all_of(begin(board), end(board), [](const CodedRank& codes) {
    int column_counter{0};
    std::for_each(begin(codes), end(codes),
                  [&column_counter](const Code& code) {
                    column_counter += GetCoveredColumns(code);
                  });
    return column_counter == 8;
  });
}

inline void SetUpEmptyBoard(BoardContainer& board) {
  board.fill(std::vector<Code>{8});
}

class EmptyBoardFixture : public ::testing::Test {
 public:
  void SetUp() { SetUpEmptyBoard(state_.board_); }
  void TearDown() { EXPECT_TRUE(BoardIsConsistent(state_.board_)); }

  State state_{};
};

}  // namespace detail
}  // namespace Chess

#endif
