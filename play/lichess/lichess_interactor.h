#ifndef LICHESS_LICHESS_INTERACTOR_H
#define LICHESS_LICHESS_INTERACTOR_H

#include "play/bubikopf_facade.h"

#include <iostream>
#include <string>

namespace Chess
{

class LichessInteractor
{
  public:
    LichessInteractor() {}

    std::string StartGame(const bool play_as_white);
    std::string RespondTo(const std::string& moves);

  private:
    BubikopfFacade& bubikopf{BubikopfFacade::GetInstance()};
};

}  // namespace Chess

#endif
