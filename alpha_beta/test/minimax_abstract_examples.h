#ifndef ALPHA_BETA_ABSTRACT_EXAMPLES_H
#define ALPHA_BETA_ABSTRACT_EXAMPLES_H

#include "alpha_beta/minimax.h"

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

Node<float> generate_negative_tree() {
  Node<float> unit{0.0F};
  const int depth{3};
  populate<float>(unit, depth);

  unit.children_.at(0)->children_.at(0)->children_.at(0)->state_ = -1.0F;
  unit.children_.at(0)->children_.at(0)->children_.at(1)->state_ = -2.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(0)->state_ = -3.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(1)->state_ = -4.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(0)->state_ = -5.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(1)->state_ = -6.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(0)->state_ = -7.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(1)->state_ = -8.0F;

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

  unit.children_.at(0)->children_.at(0)->children_.at(0)->state_ = 1.0F;
  unit.children_.at(0)->children_.at(0)->children_.at(1)->state_ = 2.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(0)->state_ = 3.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(1)->state_ = 4.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(0)->state_ = 5.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(1)->state_ = 6.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(0)->state_ = 7.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(1)->state_ = 8.0F;

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
  //             7     8   9    10   11   12   13   14            |

  return unit;
}

Node<float> generate_typical_tree() {
  Node<float> unit{0.0F};
  const int depth{3};
  populate<float>(unit, depth);

  unit.children_.at(0)->children_.at(0)->children_.at(0)->state_ = -1.0F;
  unit.children_.at(0)->children_.at(0)->children_.at(1)->state_ = 3.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(0)->state_ = 5.0F;
  unit.children_.at(0)->children_.at(1)->children_.at(1)->state_ = 42.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(0)->state_ = -6.0F;
  unit.children_.at(1)->children_.at(0)->children_.at(1)->state_ = -4.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(0)->state_ = 43.0F;
  unit.children_.at(1)->children_.at(1)->children_.at(1)->state_ = 44.0F;

  return unit;
}

}  // namespace AlphaBeta

#endif
