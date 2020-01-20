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

int CountChildren(const Node& node) {
  if (node.children_.size()) {
    int sum{0};
    for (const auto& child : node.children_) {
      sum += CountChildren(*child);
    }
    return sum + 1;
  } else {
    return 1;
  }
}

}  // namespace Chess
