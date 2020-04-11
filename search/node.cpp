#include "search/node.h"
#include "board/pieces.h"

namespace Chess {

namespace {

Position SetUpStartPosition() {
  Position position{};
  // Pawns
  for (std::int8_t col{0}; col < 8; col++) {
    position.board_.Set({col, 1}, Pawn::OfSide(Player::min));
    position.board_.Set({col, 6}, Pawn::OfSide(Player::max));
  }
  // Rooks
  position.board_.Set({0, 0}, Rook::OfSide(Player::min));
  position.board_.Set({7, 0}, Rook::OfSide(Player::min));
  position.board_.Set({0, 7}, Rook::OfSide(Player::max));
  position.board_.Set({7, 7}, Rook::OfSide(Player::max));
  // Knights
  position.board_.Set({1, 0}, Knight::OfSide(Player::min));
  position.board_.Set({6, 0}, Knight::OfSide(Player::min));
  position.board_.Set({1, 7}, Knight::OfSide(Player::max));
  position.board_.Set({6, 7}, Knight::OfSide(Player::max));
  // Bishops
  position.board_.Set({2, 0}, Bishop::OfSide(Player::min));
  position.board_.Set({5, 0}, Bishop::OfSide(Player::min));
  position.board_.Set({2, 7}, Bishop::OfSide(Player::max));
  position.board_.Set({5, 7}, Bishop::OfSide(Player::max));
  // Queens
  position.board_.Set({3, 0}, Queen::OfSide(Player::min));
  position.board_.Set({3, 7}, Queen::OfSide(Player::max));
  // Kings
  position.board_.Set({4, 0}, King::OfSide(Player::min));
  position.board_.Set({4, 7}, King::OfSide(Player::max));

  return position;
}

}  // namespace

Node::Node() { position_ = SetUpStartPosition(); }

Node::Node(const Position& position) : position_(position) {}

Node::Node(Position&& position) : position_(std::move(position)) {}

int CountNodes(const Node& node) {
  if (node.children_.size()) {
    int sum{0};
    for (const auto& child : node.children_) {
      sum += CountNodes(*child);
    }
    return sum + 1;
  } else {
    return 1;
  }
}

NodePtr ChooseChild(const std::size_t idx, NodePtr&& node) {
  Chess::NodePtr tmp{};
  tmp.swap(node);
  node.swap(tmp->children_.at(idx));
  return std::move(node);
}

}  // namespace Chess
