#ifndef PLAY_BUBIKOPF_H
#define PLAY_BUBIKOPF_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"

#include <string>
#include <vector>

namespace Chess
{

class Bubikopf
{
  public:
    void RestartGame();
    void UpdateBoard(const std::vector<std::string>& move_list);
    std::string FindBestMove();

  private:
    Position position_{};
    MoveList move_list_{};
};

}  // namespace Chess

#endif
