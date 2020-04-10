#include "bubikopf_facade.h"

#include "play/uci_utilities.h"
#include "search/minimax.h"
#include "search/populate.h"

#include <algorithm>
#include <map>
#include <vector>

namespace Chess {
namespace {

void CheckPlausibility(const std::vector<std::string>& new_moves,
                       const std::vector<std::string>& last_moves) {
  const bool new_moves_to_few = new_moves.size() < last_moves.size();
  const bool new_moves_to_many = new_moves.size() > 1 + last_moves.size();
  if (new_moves_to_few || new_moves_to_many) {
    std::cerr << "New move list not plausible." << std::endl;
    throw std::string{"New move list not plausible."};
  }
}

void CheckPlausibility(const std::string& new_move) {
  const bool is_regular_move = new_move.size() == 4;
  const bool is_promotion = new_move.size() == 5;
  if (!(is_regular_move || is_promotion)) {
    std::cerr << "New move not plausible." << std::endl;
    throw std::string{"New move not plausible."};
  }
}

SquareId GetSquareId(const char& uci_promotion_letter, const Player side) {
  if (side == Player::max) {
    const std::map<char, SquareId> map_to_white_square_id{
        {'n', SquareId::WhiteKnight},
        {'b', SquareId::WhiteBishop},
        {'r', SquareId::WhiteRook},
        {'q', SquareId::WhiteQueen},
    };
    return map_to_white_square_id.at(uci_promotion_letter);
  } else {
    const std::map<char, SquareId> map_to_black_square_id{
        {'n', SquareId::BlackKnight},
        {'b', SquareId::BlackBishop},
        {'r', SquareId::BlackRook},
        {'q', SquareId::BlackQueen},
    };
    return map_to_black_square_id.at(uci_promotion_letter);
  }
}

bool IsCorrectPromotion(const std::optional<SquareId>& expected_promotion,
                        const SquareId& target_square_piece_id) {
  if (expected_promotion) {
    return *expected_promotion == target_square_piece_id;
  } else {
    return true;
  }
}

std::string GetPromotionNotation(std::optional<SquareId> promotion) {
  const std::map<SquareId, char> map_to_promotion_notation{
      {SquareId::WhiteKnight, 'n'}, {SquareId::WhiteBishop, 'b'},
      {SquareId::WhiteRook, 'r'},   {SquareId::WhiteQueen, 'q'},
      {SquareId::BlackKnight, 'n'}, {SquareId::BlackBishop, 'b'},
      {SquareId::BlackRook, 'r'},   {SquareId::BlackQueen, 'q'},
  };
  std::string notation{};
  if (promotion) {
    notation.push_back(map_to_promotion_notation.at(*promotion));
  }
  return notation;
}

std::string ExtractUciMove(const NodePtr& root, const NodePtr& child) {
  constexpr std::size_t INVALID{99};
  std::size_t start_square_idx{INVALID};
  std::size_t target_square_idx{INVALID};

  for (std::size_t idx{0}; idx < 64; idx++) {
    const bool occupied_by_active_player_at_root =
        root->state_.board_.Get(idx)->IsOfSide(root->state_.GetTurn());
    const bool free_at_child = child->state_.board_.Get(idx)->IsEmpty();
    if (occupied_by_active_player_at_root && free_at_child) {
      start_square_idx = idx;
      std::cout << "start square " << start_square_idx << std::endl;
    }
    const bool not_occupied_by_active_player_at_root =
        !root->state_.board_.Get(idx)->IsOfSide(root->state_.GetTurn());
    const bool occupied_at_child =
        child->state_.board_.Get(idx)->IsOfSide(root->state_.GetTurn());
    if (not_occupied_by_active_player_at_root && occupied_at_child) {
      target_square_idx = idx;
      std::cout << "target square " << target_square_idx << std::endl;
    }
  }

  if ((start_square_idx == INVALID) || (target_square_idx == INVALID)) {
    std::cerr << "ExtractUciMove failed." << std::endl;
    throw std::string{"ExtractUciMove failed."};
  }

  const bool piece_id_changes =
      root->state_.board_.Get(start_square_idx)->GetId() !=
      child->state_.board_.Get(target_square_idx)->GetId();
  std::optional<SquareId> promotion{};
  if (piece_id_changes) {
    promotion = child->state_.board_.Get(target_square_idx)->GetId();
  }

  return std::string{ToUciSquare(start_square_idx) +
                     ToUciSquare(target_square_idx) +
                     GetPromotionNotation(promotion)};
}

}  // namespace

std::string BubikopfFacade::StartGame(const bool play_as_white) {
  std::cout << "BubikopfFacade::StartGame" << std::endl;
  NodePtr new_game{std::make_unique<Node>()};
  std::swap(node_, new_game);

  if (play_as_white) {
    return MakeMove();
  } else {
    std::cout << "Returning null move" << std::endl;
    return NULL_MOVE_;
  }
}

std::string BubikopfFacade::RespondTo(const std::string& moves) {
  std::cout << "BubikopfFacade::RespondTo" << std::endl;
  std::vector<std::string> new_moves = SeparateEachMove(moves);
  CheckPlausibility(new_moves, last_moves_);
  const bool new_move_was_played = new_moves.size() == 1 + last_moves_.size();
  if (new_move_was_played) {
    last_moves_ = std::move(new_moves);
    CheckPlausibility(last_moves_.back());
    ConsiderTheirMove(last_moves_.back());
    return MakeMove();
  } else {
    std::cout << "Returning null move" << std::endl;
    return NULL_MOVE_;
  }
}

std::string BubikopfFacade::MakeMove() {
  std::cout << "BubikopfFacade::MakeMove" << std::endl;
  populate(*node_, DEPTH_);
  std::vector<Evaluation> branch_evaluations{};
  for (const auto& child : node_->children_) {
    branch_evaluations.push_back(minimax(*child, DEPTH_, MIN_EVAL, MAX_EVAL));
  }

  std::vector<Evaluation>::iterator chosen_move{};
  if (node_->state_.GetTurn() == Player::max) {
    chosen_move =
        std::max_element(branch_evaluations.begin(), branch_evaluations.end());
  } else {
    chosen_move =
        std::min_element(branch_evaluations.begin(), branch_evaluations.end());
  }

  if (chosen_move != branch_evaluations.end()) {
    const std::size_t move_idx =
        static_cast<std::size_t>(chosen_move - branch_evaluations.begin());
    last_moves_.push_back(ExtractUciMove(node_, node_->children_.at(move_idx)));
    node_ = ChooseChild(move_idx, std::move(node_));
  } else {
    std::cerr << "Error during MakeMove" << std::endl;
    throw std::string{"Error during MakeMove"};
  }

  std::cout << "Bubikopf plays " << last_moves_.back() << std::endl;
  std::cout << node_->state_.board_ << std::flush;

  return last_moves_.back();
}

void BubikopfFacade::ConsiderTheirMove(const std::string& move) {
  std::cout << "BubikopfFacade::ConsiderTheirMove" << std::endl;
  populate(*node_, DEPTH_);
  const std::string start_square{move.substr(0, 2)};
  const std::string target_square{move.substr(2, 2)};
  std::optional<SquareId> promotion{};
  if (move.size() == 5) {
    const Player their_color = !node_->state_.GetTurn();
    promotion = GetSquareId(move.at(5), their_color);
  }

  for (std::size_t idx{0}; idx < node_->children_.size(); ++idx) {
    const auto& child = node_->children_.at(idx);
    const bool start_square_is_empty =
        child->state_.board_.Get(ToIdx(start_square))->IsEmpty();
    const bool target_square_is_occupied =
        !child->state_.board_.Get(ToIdx(target_square))->IsEmpty();
    const bool correct_promotion = IsCorrectPromotion(
        promotion, child->state_.board_.Get(ToIdx(target_square))->GetId());
    if (start_square_is_empty && target_square_is_occupied &&
        correct_promotion) {
      node_ = ChooseChild(idx, std::move(node_));
      break;
    }
  }

  std::cout << "They play " << move << std::endl;
  std::cout << node_->state_.board_ << std::flush;
}

}  // namespace Chess
