#include "search/node.h"

#include <gtest/gtest.h>

#include <algorithm>

namespace Chess {
namespace {

TEST(Node, Given_DefaultCtor_ExpectStartPosition) {
  const Node node{};
  std::cout << node.state_.board_;
  // check manually
}

TEST(CountNodesTest, GivenNoChildren_Expect1) {
  // Setup
  Node root_node{State{}};

  // Call
  const int returned_number_of_nodes{CountNodes(root_node)};

  // Expect
  const int expected_number_of_nodes{1};
  EXPECT_EQ(returned_number_of_nodes, expected_number_of_nodes);
}

TEST(CountNodesTest, Given7Children_Expect8) {
  // Setup
  Node root_node{State{}};
  root_node.children_.resize(7);
  std::generate(root_node.children_.begin(), root_node.children_.end(),
                []() { return std::make_unique<Node>(State{}); });

  // Call
  const int returned_number_of_nodes{CountNodes(root_node)};

  // Expect
  const int expected_number_of_nodes{8};
  EXPECT_EQ(returned_number_of_nodes, expected_number_of_nodes);
}

TEST(CountNodesTest, Given2ChildrenAnd4GrandChildren_Expect7) {
  // Setup
  Node root_node{State{}};
  root_node.children_.push_back(std::make_unique<Node>(State{}));
  root_node.children_.push_back(std::make_unique<Node>(State{}));
  root_node.children_.at(0)->children_.push_back(
      std::make_unique<Node>(State{}));
  root_node.children_.at(0)->children_.push_back(
      std::make_unique<Node>(State{}));
  root_node.children_.at(1)->children_.push_back(
      std::make_unique<Node>(State{}));
  root_node.children_.at(1)->children_.push_back(
      std::make_unique<Node>(State{}));

  // Call
  const int returned_number_of_nodes{CountNodes(root_node)};

  // Expect
  const int expected_number_of_nodes{7};
  EXPECT_EQ(returned_number_of_nodes, expected_number_of_nodes);
}

}  // namespace
}  // namespace Chess
