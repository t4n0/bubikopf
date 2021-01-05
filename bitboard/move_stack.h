#ifndef BITBOARD_MOVE_STACK_H
#define BITBOARD_MOVE_STACK_H

#include "bitboard/move.h"

#include <array>
#include <sstream>

namespace Chess
{

/// @brief Type to preallocate memory for multiple layers of candidate moves during move generation.
///
/// These moves will be used during search to update the current position one move at a time (also taking it back).
using MoveStack = std::array<Bitmove, 1000>;

inline std::string ToString(const MoveStack& move_stack)
{
    std::stringstream move_stack_print_out{};
    move_stack_print_out << "Move stack consists of:\n";
    for (const auto move : move_stack)
    {
        const std::string move_print_out = ToString(move);
        if (!move_print_out.empty())
        {
            move_stack_print_out << move_print_out;
        }
    }
    return move_stack_print_out.str();
}

}  // namespace Chess

#endif