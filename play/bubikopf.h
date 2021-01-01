#ifndef PLAY_BUBIKOPF_H
#define PLAY_BUBIKOPF_H

#include "bitboard/move_list.h"
#include "bitboard/position.h"
#include "play/logger.h"

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
    Logger logger{"log_bubikopf_engine.txt"};
};

}  // namespace Chess

#endif
