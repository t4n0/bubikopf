#ifndef PLAY_LICHESS_INTERACTOR_H
#define PLAY_LICHESS_INTERACTOR_H

#include "play/bubikopf_facade.h"

#include <iostream>
#include <string>

namespace Chess {

class LichessInteractor {
 public:
  LichessInteractor() {}

  std::string StartGame(const bool play_as_white);
  std::string RespondTo(const std::string& moves);

 private:
  BubikopfFacade& bubikopf{BubikopfFacade::getInstance()};
};

}  // namespace Chess

#endif
