#ifndef SEARCH_TEST_MOVE_GENERATION_MOCK_H
#define SEARCH_TEST_MOVE_GENERATION_MOCK_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <type_traits>

namespace Chess
{

struct GenerateTwoMovesWithUniqueDebugId
{
    static constexpr bool generate_two_moves_with_unique_debug_id{true};
};

template <typename Behavior = GenerateTwoMovesWithUniqueDebugId>
std::enable_if_t<Behavior::generate_two_moves_with_unique_debug_id, MoveList::iterator> GenerateMoves(
    const Position& /*unused*/,
    MoveList::iterator move_generation_insertion_iterator)
{
    static Bitmove unique_id{1};
    *move_generation_insertion_iterator++ = unique_id++;
    *move_generation_insertion_iterator++ = unique_id++;
    return move_generation_insertion_iterator;
}

}  // namespace Chess

#endif
