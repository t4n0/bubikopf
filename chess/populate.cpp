#include "chess/populate.h"

namespace Chess {
namespace detail {

std::vector<NodePtr> find_legal_moves(const Node& node) {
  constexpr std::array<Row, 8> rows{0, 1, 2, 3, 4, 5, 6, 7};
  constexpr std::array<Column, 8> cols{0, 1, 2, 3, 4, 5, 6, 7};

  std::vector<NodePtr> legal_moves{};
  for (const Row row : rows) {
    for (const Column col : cols) {
      const Coordinate location{col, row};
      const Piece piece{node.state_.board_.Get(location)};

      std::vector<NodePtr> piece_moves{};
      switch (piece) {
        case Piece::e:
          break;
        case Piece::K:
        case Piece::k:
          piece_moves = find_moves<Piece::k>(node, location);
          break;
        case Piece::P:
        case Piece::p:
          piece_moves = find_moves<Piece::p>(node, location);
          break;
        case Piece::B:
        case Piece::b:
          piece_moves = find_moves<Piece::b>(node, location);
          break;
        case Piece::N:
        case Piece::n:
          piece_moves = find_moves<Piece::n>(node, location);
          break;
        case Piece::R:
        case Piece::r:
          piece_moves = find_moves<Piece::r>(node, location);
          break;
        case Piece::Q:
        case Piece::q:
          piece_moves = find_moves<Piece::q>(node, location);
          break;
      }

      for (NodePtr& node : piece_moves) {
        legal_moves.emplace_back(std::move(node));
      }
    }
  }

  return legal_moves;
}

}  // namespace detail
}  // namespace Chess
