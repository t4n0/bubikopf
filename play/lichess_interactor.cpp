#include "play/lichess_interactor.h"

namespace Chess {

std::string LichessInteractor::StartGame(const bool play_as_white) {
  std::cout << play_as_white << '\n';
  return "0000";
}

std::string LichessInteractor::RespondTo(const std::string& moves) {
  std::cout << moves << '\n';
  return "0000";
}

}  // namespace Chess
