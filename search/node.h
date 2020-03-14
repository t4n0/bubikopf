#ifndef SEARCH_NODE_H
#define SEARCH_NODE_H

#include "board/state.h"

#include <memory>
#include <vector>

namespace Chess {

class Node {
 public:
  Node(State&& state) : state_(std::move(state)) {}

  std::vector<std::unique_ptr<Node>> children_{};
  State state_;
};

inline int CountNodes(const Node& node) {
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

}  // namespace Chess

#endif
