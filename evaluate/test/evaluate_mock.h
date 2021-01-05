#ifndef EVALUATE_TEST_EVALUATE_MOCK_H
#define EVALUATE_TEST_EVALUATE_MOCK_H

#include <type_traits>

namespace Chess
{

struct EvaluteToZero
{
    static constexpr bool evaluate_to_zero{true};
};

template <typename Behaviour>
std::enable_if_t<Behaviour::evaluate_to_zero, Evaluation> Evaluate(const Position& /*unused*/)
{
    return Evaluation{0};
}

}  // namespace Chess

#endif
