#ifndef CHESS_POPULATE_H
#define CHESS_POPULATE_H

#include <chess/state.h>
#include <game_tree/minimax.h>

namespace Chess {
namespace detail {

std::vector<NodePtr> find_pawn_capture_moves(const Node& node);
std::vector<NodePtr> find_legal_moves(const Node& node);

}  // namespace detail
}  // namespace Chess

namespace GameTree {

template <>
void populate(Node<Chess::State>& node, const int depth) {
  if (!depth) {
    node.children_ = Chess::detail::find_legal_moves(node);
  } else {
    for (std::unique_ptr<Node<Chess::State>>& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace GameTree

#endif
