#include <chess/board_test_helpers.h>
#include <chess/populate.h>

#include <gtest/gtest.h>

namespace Chess {
namespace {

class FindLegalMovesTest : public detail::EmptyBoardFixture {};

TEST_F(FindLegalMovesTest, MustNotThrow) {
  std::unique_ptr<Node> unit{std::make_unique<Node>(unit_)};
  unit->children_ = detail::find_legal_moves(*unit);
}

}  // namespace
}  // namespace Chess
