#include "chess/populate.h"

#include <algorithm>

namespace Chess {

std::vector<NodePtr> collect_moves_from_all_pieces(const Node& node) {
  std::vector<NodePtr> legal_moves{};
  for (const SquarePtr& square : node.state_.board_.squares_) {
    std::vector<NodePtr>
        piece_moves{};  //= square->FindMoves(coordinate, node);

    for (NodePtr& node : piece_moves) {
      legal_moves.emplace_back(std::move(node));
    }
  }

  return legal_moves;
}

}  // namespace Chess
