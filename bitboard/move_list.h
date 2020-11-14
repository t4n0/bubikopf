#ifndef BITBOARD_MOVE_LIST_H
#define BITBOARD_MOVE_LIST_H

#include "bitboard/move.h"

#include <sstream>
#include <array>

namespace Chess
{

/// TODO: Throw when trying to assign past the end.
/// @brief Type to preallocate array for move generation
using MoveList = std::array<Bitmove, 1000>;

inline std::string ToString(const MoveList& move_list)
{
    std::stringstream move_list_print_out{};
    move_list_print_out << "Move list consists of:\n";
    for (const auto move : move_list)
    {
        const std::string move_print_out = ToString(move);
        if (!move_print_out.empty())
        {
            move_list_print_out << move_print_out;
        }
    }
    return move_list_print_out.str();
}

}  // namespace Chess

#endif