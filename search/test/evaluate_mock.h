#ifndef SEARCH_TEST_EVALUATE_MOCK_H
#define SEARCH_TEST_EVALUATE_MOCK_H

#include "evaluate/evaluate.h"

#include <type_traits>
#include <vector>

namespace Chess
{

static std::vector<int> ORDER_OF_NODE_EVALUATION{};

template <typename Behaviour>
std::enable_if_t<Behaviour::is_mock, Evaluation> evaluate(const Position& position)
{
    const int hidden_node_identifier = position.static_plies_;
    ORDER_OF_NODE_EVALUATION.push_back(hidden_node_identifier);
    return Behaviour::GetNodeValue(hidden_node_identifier);
};

struct EvaluteToZeroAndCount
{
    static constexpr bool evaluate_to_zero{true};
    static int number_of_evaluations;
};

template <typename Behaviour>
std::enable_if_t<Behaviour::evaluate_to_zero, Evaluation> evaluate(const PositionWithBitboards& /*unused*/)
{
    EvaluteToZeroAndCount::number_of_evaluations++;
    return 0.0F;
}

}  // namespace Chess

#endif
