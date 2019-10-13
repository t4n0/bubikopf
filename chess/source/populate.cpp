#include <chess/populate.h>

namespace Chess {
namespace detail {

std::vector<NodePtr> find_pawn_capture_moves(const Node& node) {
  std::vector<NodePtr> test{};
  test.emplace_back(std::make_unique<Node>(Chess::State{}));
  test.at(0)->state_.turn_ = GameTree::Player::max;

  return test;
}

std::vector<NodePtr> find_legal_moves(const Node& node) {
  std::vector<NodePtr> test1 = find_pawn_capture_moves(node);
  std::vector<NodePtr> test2 = find_pawn_capture_moves(node);

  // find_king_capture_moves();
  // find_knight_capture_moves();
  // find_bishop_capture_moves();
  // find_rook_capture_moves();
  // find_queen_capture_moves();

  // find_pawn_non_capture_moves();
  // find_king_non_capture_moves();
  // find_knight_non_capture_moves();
  // find_bishop_non_capture_moves();
  // find_rook_non_capture_moves();
  // find_queen_non_capture_moves();

  return std::vector<NodePtr>{};
}

}  // namespace detail
}  // namespace Chess
