#ifndef SEARCH_TEST_EVALUATE_MOCK_H
#define SEARCH_TEST_EVALUATE_MOCK_H

#include "search/evaluate.h"

#include <type_traits>
#include <vector>

namespace Chess {

static std::vector<int> ORDER_OF_NODE_EVALUATION{};

template <typename Behaviour>
std::enable_if_t<Behaviour::is_mock, Evaluation> evaluate(const State& state) {
  const int hidden_node_identifier = state.plies_;
  ORDER_OF_NODE_EVALUATION.push_back(hidden_node_identifier);
  return Behaviour::GetNodeValue(hidden_node_identifier);
};

}  // namespace Chess

#endif
