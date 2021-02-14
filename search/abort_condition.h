#ifndef SEACH_ABORT_CONDITION_H
#define SEACH_ABORT_CONDITION_H

#include <chrono>

namespace Chess
{

/// @brief A type to be thrown during search if time for pondering is over.
///
/// Regardless of current state of search.
struct CalculationWasDue
{
};

struct AbortCondition
{
    std::size_t full_search_depth{0};
    std::chrono::steady_clock::time_point calculation_is_due{std::chrono::steady_clock::time_point::max()};
};

}  // namespace Chess

#endif
