#include "search/populate.h"

#include <algorithm>

namespace Chess {

void populate(Node& node, const int depth) {
  if (depth) {
    if (!node.children_.size()) {
      std::vector<Position> child_positions = node.position_.FindPlies();
      std::for_each(child_positions.begin(), child_positions.end(),
                    [&node](Position& position) {
                      node.children_.emplace_back(
                          std::make_unique<Node>(std::move(position)));
                    });
    }

    for (NodePtr& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace Chess
