#include "chess/populate.h"

#include <algorithm>

namespace Chess {

std::vector<NodePtr> collect_moves_from_all_pieces(const Node& node) {
  std::vector<NodePtr> legal_moves{};
  for (std::size_t idx{0}; idx < node.state_.board_.squares_.size(); ++idx) {
    const Square& square = *node.state_.board_.squares_.at(idx);
    std::vector<State> piece_moves = square.FindMoves(idx, node.state_);

    legal_moves.insert(legal_moves.end(), piece_moves.begin(),
                       piece_moves.end());
  }

  return legal_moves;
}

}  // namespace Chess
