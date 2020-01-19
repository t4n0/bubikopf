#ifndef CHESS_POPULATE_H
#define CHESS_POPULATE_H

#include "alpha_beta/minimax.h"
#include "chess/state.h"

namespace Chess {

using Node = AlphaBeta::Node<State>;
using NodePtr = std::unique_ptr<Node>;

std::vector<NodePtr> collect_plies_from_all_pieces(const Chess::Node&);

int CountChildren(const Node& node);

}  // namespace Chess

namespace AlphaBeta {

template <>
inline void populate(Chess::Node& node, const int depth) {
  if (depth) {
    if (!node.children_.size()) {
      node.children_ = Chess::collect_plies_from_all_pieces(node);
    }

    for (Chess::NodePtr& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace AlphaBeta

#endif
