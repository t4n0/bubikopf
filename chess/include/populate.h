#ifndef CHESS_POPULATE_H
#define CHESS_POPULATE_H

#include <alpha_beta/minimax.h>
#include <chess/state.h>

namespace Chess {
namespace detail {

template <Piece>
std::vector<NodePtr> find_moves(const Node& node, const Coordinate& coordinate);

template <>
std::vector<NodePtr> find_moves<Piece::e>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::k>(const Node& node,
                                          const Coordinate& location) {
  const Piece piece{node.state_.board_.Get(location)};
  if (IsAPieceOfSide(piece, node.state_.turn_)) {
    /// todo: add castling moves
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

      /// todo: check that king is not in check
      if (IsOnTheBoard(destination) &&
          !IsAPieceOfSide(node.state_.board_.Get(destination),
                          node.state_.turn_)) {
        Chess::State state_after_move{state_before_move};
        state_after_move.board_.Set(destination,
                                    state_after_move.board_.Get(location));
        state_after_move.board_.Set(location, Piece::e);
        moves.emplace_back(std::make_unique<Node>(std::move(state_after_move)));
      }
    }

    return moves;
  } else
    return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::n>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::b>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::r>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::q>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::K>(const Node& node,
                                          const Coordinate& location) {
  return find_moves<Piece::k>(node, location);
}

template <>
std::vector<NodePtr> find_moves<Piece::N>(const Node& node,
                                          const Coordinate& location) {
  return find_moves<Piece::n>(node, location);
}

template <>
std::vector<NodePtr> find_moves<Piece::B>(const Node& node,
                                          const Coordinate& location) {
  return find_moves<Piece::b>(node, location);
}

template <>
std::vector<NodePtr> find_moves<Piece::R>(const Node& node,
                                          const Coordinate& location) {
  return find_moves<Piece::r>(node, location);
}

template <>
std::vector<NodePtr> find_moves<Piece::Q>(const Node& node,
                                          const Coordinate& location) {
  return find_moves<Piece::q>(node, location);
}

template <>
std::vector<NodePtr> find_moves<Piece::p>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

template <>
std::vector<NodePtr> find_moves<Piece::P>(const Node& /*unsused*/,
                                          const Coordinate& /*unsused*/) {
  return {};
}

std::vector<NodePtr> find_legal_moves(const Node& node);

}  // namespace detail
}  // namespace Chess

namespace GameTree {

template <>
void populate(Node<Chess::State>& node, const int depth) {
  if (!depth) {
    node.children_ = Chess::detail::find_legal_moves(node);
  } else {
    for (std::unique_ptr<Node<Chess::State>>& child : node.children_) {
      populate(*child, depth - 1);
    }
  }
}

}  // namespace GameTree

#endif
