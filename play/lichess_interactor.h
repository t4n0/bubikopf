#ifndef PLAY_LICHESS_INTERACTOR_H
#define PLAY_LICHESS_INTERACTOR_H

#include <iostream>
#include <string>

namespace Chess {

class LichessInteractor {
 public:
  std::string StartGame(const bool play_as_white);
  std::string RespondTo(const std::string& moves);
};

}  // namespace Chess

#endif
