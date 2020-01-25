#ifndef SEARCH_POPULATE_H
#define CHESS_POPULATE_H

#include "board/state.h"
#include "search/minimax.h"

namespace Chess {

using NodePtr = std::unique_ptr<Node<State>>;

std::vector<NodePtr> collect_plies_from_all_pieces(const Node<State>&);

int CountChildren(const Node<State>& node);

template <>
inline void populate(Node<State>& node, const int depth) {
  if (depth) {
    if (!node.children_.size()) {
      node.children_ = collect_plies_from_all_pieces(node);
    }

    for (NodePtr& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace Chess

#endif
