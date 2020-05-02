#ifndef BITBOARD_MOVE_GENERATION_H
#define BITBOARD_MOVE_GENERATION_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

namespace Chess {

/// @brief Generates all legal moves from position
/// @returns An iterator pointing to the element past the last generated move
move_list_t::iterator GenerateMoves(const PositionWithBitboards& position,
                                    move_list_t::iterator next_to_generate);
}  // namespace Chess

#endif
