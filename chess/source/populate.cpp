#include <chess/populate.h>

namespace Chess {
namespace detail {

template <Piece>
std::vector<NodePtr> find_moves(const Node& node, const Coordinate& coordinate);

template <>
std::vector<NodePtr> find_moves<Piece::k>(const Node& node,
                                          const Coordinate& location) {
  /// todo: castling moves are missing
  constexpr std::array<Coordinate, 8> king_directions{
      {{1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}}};

  Chess::State state_before_move{node.state_};
  state_before_move.turn_ = node.state_.turn_ == GameTree::Player::max
                                ? GameTree::Player::min
                                : GameTree::Player::max;
  state_before_move.plies_++;
  state_before_move.static_plies_++;

  std::vector<NodePtr> moves{};
  for (const Coordinate& direction : king_directions) {
    const Coordinate destination = location + direction;

    /// todo: king is not in check
    if (IsOnTheBoard(destination) &&
        IsAPieceOfSide(node.state_.board_.Get(destination),
                       node.state_.turn_)) {
      Chess::State state_after_move{state_before_move};
      state_after_move.board_.Set(location, Piece::e);
      state_after_move.board_.Set(destination, Piece::k);
      NodePtr king_move = std::make_unique<Node>(std::move(state_after_move));
      moves.push_back(std::move(king_move));
    }
  }

  return moves;
}

std::vector<NodePtr> find_legal_moves(const Node& node) {
  constexpr std::array<Row, 8> rows{0, 1, 2, 3, 4, 5, 6, 7};
  constexpr std::array<Column, 8> cols{0, 1, 2, 3, 4, 5, 6, 7};

  std::vector<NodePtr> legal_moves{};
  for (const Row row : rows) {
    for (const Column col : cols) {
      const Piece piece{node.state_.board_.Get({col, row})};
      if (IsAPieceOfSide(piece, node.state_.turn_)) {
        std::vector<NodePtr> piece_moves{
            find_moves<Piece::k>(node, {col, row})};
        for (NodePtr& node : piece_moves) {
          legal_moves.emplace_back(std::move(node));
        }
      }
    }
  }

  return legal_moves;
}

}  // namespace detail
}  // namespace Chess
