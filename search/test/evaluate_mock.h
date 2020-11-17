#ifndef SEARCH_TEST_EVALUATE_MOCK_H
#define SEARCH_TEST_EVALUATE_MOCK_H

#include "evaluate/evaluate.h"

#include <type_traits>
#include <vector>

namespace Chess
{

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
