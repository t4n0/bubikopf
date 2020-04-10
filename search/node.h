#ifndef SEARCH_NODE_H
#define SEARCH_NODE_H

#include "board/state.h"

#include <memory>
#include <vector>

namespace Chess {

class Node {
 public:
  Node();
  Node(const State& state);

  Node(const Node&) = delete;
  Node(Node&&) = delete;
  Node& operator=(Node) = delete;
  Node& operator=(Node&&) = delete;
  ~Node() = default;

  std::vector<std::unique_ptr<Node>> children_{};
  State state_;
};

using NodePtr = std::unique_ptr<Node>;

int CountNodes(const Node& node);

NodePtr ChooseChild(const std::size_t idx, NodePtr&& node);

}  // namespace Chess

#endif
