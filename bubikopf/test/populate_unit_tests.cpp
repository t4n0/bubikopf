#include <chess/populate.h>

#include <gtest/gtest.h>

namespace Chess {
namespace {

TEST(FindPawnCaptureMoves, MustNotThrow) {
  std::unique_ptr<Node> unit{std::make_unique<Node>(Chess::State{})};
  unit->children_ = detail::find_pawn_capture_moves(*unit);
}

}  // namespace
}  // namespace Chess
