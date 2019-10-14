#include <chess/populate.h>

namespace Chess {
namespace detail {

std::vector<NodePtr> find_legal_moves(const Node& node) {
  for (const File& file : FILES) {
    for (const Rank& rank : RANKS) {
      const Piece piece{node.state_.GetPieceFrom(file, rank)};
      if (IsAPieceOfSide(piece, node.state_.turn_)) {
        // find_moves<code>(node, {file, rank});
        return {};
      }
    }
  }

  return std::vector<NodePtr>{};
}

}  // namespace detail
}  // namespace Chess
