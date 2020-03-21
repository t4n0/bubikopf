#include "search/node.h"

namespace Chess {

int CountNodes(const Node& node) {
  if (node.children_.size()) {
    int sum{0};
    for (const auto& child : node.children_) {
      sum += CountNodes(*child);
    }
    return sum + 1;
  } else {
    return 1;
  }
}

NodePtr ChooseChild(const std::size_t idx, NodePtr&& node) {
  Chess::NodePtr tmp{};
  tmp.swap(node);
  node.swap(tmp->children_.at(idx));
  return std::move(node);
}

}  // namespace Chess
