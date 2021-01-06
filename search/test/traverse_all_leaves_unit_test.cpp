#include "search/traverse_all_leaves.h"

#include <gtest/gtest.h>

#include <array>
#include <type_traits>

namespace Chess
{

struct GenerateTwoMovesWithUniqueDebugId
{
    static constexpr bool generate_two_moves_with_unique_debug_id{true};
};

template <typename Behavior = GenerateTwoMovesWithUniqueDebugId>
std::enable_if_t<Behavior::generate_two_moves_with_unique_debug_id, MoveStack::iterator> GenerateMoves(
    const Position& /*unused*/,
    MoveStack::iterator move_generation_insertion_iterator)
{
    static Bitmove unique_id{1};
    *move_generation_insertion_iterator++ = unique_id++;
    *move_generation_insertion_iterator++ = unique_id++;
    return move_generation_insertion_iterator;
}

namespace
{

TEST(MoveListTest, GivenDepth3_ExpectDebuggingIdsOf6LastVisitedMovesInMoveStack)
{
    // Setup
    Position position{};
    MoveStack move_stack{};
    Statistic stats{};
    constexpr int depth{3};

    // Call
    TraverseAllLeaves<GenerateTwoMovesWithUniqueDebugId>(position, depth, move_stack.begin(), stats);

    // Expect
    const std::array<Bitmove, 6> expected_debugging_ids{1, 2, 9, 10, 13, 14};  // worked out by hand
    for (std::size_t idx{0}; idx < expected_debugging_ids.size(); idx++)
    {
        EXPECT_EQ(expected_debugging_ids.at(idx), move_stack.at(idx));
    }
}

}  // namespace
}  // namespace Chess