#ifndef ALPHA_BETA_ABSTRACT_EXAMPLES_H
#define ALPHA_BETA_ABSTRACT_EXAMPLES_H

#include "alpha_beta/minimax.h"

#include <functional>
#include <memory>

namespace AlphaBeta {

static std::vector<float> order_of_evaluation{};

template <>
Evaluation evaluate(const float& state) {
  order_of_evaluation.push_back(state);
  return state;
};

template <>
void populate(Node<float>& node, const int depth) {
  if (depth) {
    std::unique_ptr<Node<float>> child1 = std::make_unique<Node<float>>(0.0F);
    node.children_.push_back(std::move(child1));
    std::unique_ptr<Node<float>> child2 = std::make_unique<Node<float>>(0.0F);
    node.children_.push_back(std::move(child2));

    for (const std::unique_ptr<Node<float>>& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
};

std::size_t GetBit(const int bit_position, const int value) {
  return (value >> bit_position) & 1;
}

void FillLeafNodesFromLeftToRight(Node<float>& node,
                                  std::function<int(const int)> func) {
  for (int i{0}; i < 8; i++) {
    node.children_.at(GetBit(2, i))
        ->children_.at(GetBit(1, i))
        ->children_.at(GetBit(0, i))
        ->state_ = func(i);
  }
}

Node<float> generate_negative_tree() {
  Node<float> unit{0.0F};
  const int depth{3};
  populate<float>(unit, depth);

  FillLeafNodesFromLeftToRight(unit, [](const int i) { return -i - 1; });

  // produces the following tree:                                 |
  //                                                              |
  //                               0                              |
  //                              / \                             |
  //                             /   \                            |
  //                            /     \                           |
  //                           /       \                          |
  //                          /         \                         |
  //                         /           \                        |
  //                        /             \                       |
  //                       /               \                      |
  //                      /                 \                     |
  //                     0                   0                    |
  //                    / \                 / \                   |
  //                   /   \               /   \                  |
  //                  /     \             /     \                 |
  //                 /       \           /       \                |
  //                0         0         0         0               |
  //               / \       / \       / \       / \              |
  //              /   \     /   \     /   \     /   \             |
  //             -1   -2  -3    -4   -5   -6   -7   -8            |

  return unit;
}

Node<float> generate_positive_tree() {
  Node<float> unit{0.0F};
  const int depth{3};
  populate<float>(unit, depth);

  FillLeafNodesFromLeftToRight(unit, [](const int i) { return i + 1; });

  // produces the following tree:                                 |
  //                                                              |
  //                               0                              |
  //                              / \                             |
  //                             /   \                            |
  //                            /     \                           |
  //                           /       \                          |
  //                          /         \                         |
  //                         /           \                        |
  //                        /             \                       |
  //                       /               \                      |
  //                      /                 \                     |
  //                     0                   0                    |
  //                    / \                 / \                   |
  //                   /   \               /   \                  |
  //                  /     \             /     \                 |
  //                 /       \           /       \                |
  //                0         0         0         0               |
  //               / \       / \       / \       / \              |
  //              /   \     /   \     /   \     /   \             |
  //             1     2   3     4   5     6   7     8            |

  return unit;
}

Node<float> generate_typical_tree() {
  Node<float> unit{0.0F};
  const int depth{3};
  populate<float>(unit, depth);

  const std::vector<float> leaf_evaluations{-1.0F, 3.0F,  5.0F,  42.0F,
                                            -6.0F, -4.0F, 43.0F, 44.0F};

  FillLeafNodesFromLeftToRight(unit, [&leaf_evaluations](const int i) {
    return leaf_evaluations.at(static_cast<std::size_t>(i));
  });

  return unit;
}

}  // namespace AlphaBeta

#endif
