#include "lichess/lichess_interactor.h"

namespace Chess
{

std::string LichessInteractor::StartGame(const bool play_as_white)
{
    return bubikopf.StartGame(play_as_white);
}

std::string LichessInteractor::RespondTo(const std::string& moves)
{
    return bubikopf.RespondTo(moves);
}

}  // namespace Chess
