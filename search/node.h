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

}  // namespace Chess

#endif
