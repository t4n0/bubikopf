#include "search/populate.h"

#include <algorithm>

namespace Chess {

std::vector<NodePtr> collect_plies_from_all_pieces(const Node& node) {
  std::vector<NodePtr> collected_plies{};
  for (std::size_t idx{0}; idx < node.state_.board_.squares_.size(); ++idx) {
    const ISquare& square = *node.state_.board_.squares_.at(idx);
    std::vector<State> new_plies = square.FindPlies(idx, node.state_);

    std::for_each(new_plies.begin(), new_plies.end(),
                  [&collected_plies](State& state) {
                    collected_plies.emplace_back(
                        std::make_unique<Node>(std::move(state)));
                  });
  }

  return collected_plies;
}

void populate(Node& node, const int depth) {
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