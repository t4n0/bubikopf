#ifndef BITBOARD_MOVE_LIST_H
#define BITBOARD_MOVE_LIST_H

#include "bitboard/squares.h"
#include "bitboard/pieces.h"
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
        const bool move_is_empty = !move;
        if (move_is_empty)
        {
            break;
        }
        const std::size_t source_bit = move & MOVE_MASK_SOURCE;
        const std::string source = SQUARE_LABEL.at(source_bit);
        const std::size_t target_bit = (move & MOVE_MASK_TARGET) >> MOVE_SHIFT_TARGET;
        const std::string target = SQUARE_LABEL.at(target_bit);
        const std::size_t moved_piece_kind = (move & MOVE_MASK_MOVED_PIECE) >> MOVE_SHIFT_MOVED_PIECE;
        const std::string moved_piece = PIECE_LABEL.at(moved_piece_kind);
        const std::size_t captured_piece_kind = (move & MOVE_MASK_CAPTURED_PIECE) >> MOVE_SHIFT_CAPTURED_PIECE;
        const std::string captured_piece = PIECE_LABEL.at(captured_piece_kind);
        const std::size_t promotion_kind = (move & MOVE_MASK_PROMOTION) >> MOVE_SHIFT_PROMOTION;
        const std::string promotion = PIECE_LABEL.at(promotion_kind);
        const std::size_t move_value_kind = (move & MOVE_MASK_TYPE) >> MOVE_SHIFT_TYPE;
        const std::string move_value = MOVE_VALUE_LABELS.at(move_value_kind);

        move_list_print_out << moved_piece << " on " << source << " to " << target << " promoting to " << promotion
                            << " capturing " << captured_piece << " with move value " << move_value << "\n";
    }
    return move_list_print_out.str();
}

}  // namespace Chess

#endif