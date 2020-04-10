#include "search/populate.h"

#include <algorithm>

namespace Chess {

void populate(Node& node, const int depth) {
  if (depth) {
    if (!node.children_.size()) {
      std::vector<State> child_states = node.state_.FindPlies();
      std::for_each(child_states.begin(), child_states.end(),
                    [&node](State& state) {
                      node.children_.emplace_back(
                          std::make_unique<Node>(std::move(state)));
                    });
    }

    for (NodePtr& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace Chess
