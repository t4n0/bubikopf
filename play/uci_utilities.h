#ifndef PLAY_DISASSEMBLE_UCI_MOVES_H
#define PLAY_DISASSEMBLE_UCI_MOVES_H

#include <string>
#include <vector>

namespace Chess {

std::vector<std::string> SeparateEachMove(const std::string& moves);

}  // namespace Chess

#endif  // PLAY_DISASSEMBLE_UCI_MOVES_H
