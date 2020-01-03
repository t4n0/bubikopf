#include "chess/populate.h"

#include <algorithm>

namespace Chess {

std::vector<NodePtr> collect_moves_from_all_pieces(const Node& node) {
  std::vector<NodePtr> collected_moves{};
  for (std::size_t idx{0}; idx < node.state_.board_.squares_.size(); ++idx) {
    const ISquare& square = *node.state_.board_.squares_.at(idx);
    std::vector<State> new_moves = square.FindMoves(idx, node.state_);

    std::for_each(new_moves.begin(), new_moves.end(),
                  [&collected_moves](State& state) {
                    collected_moves.emplace_back(
                        std::make_unique<Node>(std::move(state)));
                  });
  }

  return collected_moves;
}

}  // namespace Chess
