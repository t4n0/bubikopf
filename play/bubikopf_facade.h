#ifndef PLAY_BUBIKOPF_FACADE_H
#define PLAY_BUBIKOPF_FACADE_H

#include "search/node.h"

#include <string>
#include <vector>

namespace Chess {

class BubikopfFacade {
 public:
  BubikopfFacade(BubikopfFacade const&) = delete;
  void operator=(BubikopfFacade const&) = delete;
  static BubikopfFacade& GetInstance() {
    static BubikopfFacade instance;
    return instance;
  }

  std::string StartGame(const bool play_as_white);
  std::string RespondTo(const std::string& moves);

 private:
  BubikopfFacade() {}

  std::string MakeMove();
  void ConsiderTheirMove(const std::string& move);

  NodePtr node_{std::make_unique<Node>(Position{})};
  std::vector<std::string> last_moves_{};

  const uint8_t DEPTH_{4};
  const std::string NULL_MOVE_{"0000"};
};

}  // namespace Chess

#endif  // PLAY_BUBIKOPF_FACADE_H
