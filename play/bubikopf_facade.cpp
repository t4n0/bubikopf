#include "bubikopf_facade.h"

#include "play/uci_utilities.h"
#include "search/minimax.h"
#include "search/populate.h"

#include <algorithm>
#include <map>
#include <vector>

namespace Chess
{
namespace
{

void CheckPlausibility(const std::vector<std::string>& new_moves, const std::vector<std::string>& last_moves)
{
    const bool new_moves_to_few = new_moves.size() < last_moves.size();
    const bool new_moves_to_many = new_moves.size() > 1 + last_moves.size();
    if (new_moves_to_few || new_moves_to_many)
    {
        std::cerr << "New move list not plausible." << std::endl;
        throw std::string{"New move list not plausible."};
    }
}

void CheckPlausibility(const std::string& new_move)
{
    const bool is_regular_move = new_move.size() == 4;
    const bool is_promotion = new_move.size() == 5;
    if (!(is_regular_move || is_promotion))
    {
        std::cerr << "New move not plausible." << std::endl;
        throw std::string{"New move not plausible."};
    }
}

}  // namespace

std::string BubikopfFacade::StartGame(const bool play_as_white)
{
    std::cout << "BubikopfFacade::StartGame" << std::endl;
    NodePtr new_game{std::make_unique<Node>()};
    std::swap(node_, new_game);

    if (play_as_white)
    {
        return MakeMove();
    }
    else
    {
        populate(*node_, DEPTH_);
        std::cout << "Returning null move" << std::endl;
        return NULL_MOVE_;
    }
}

std::string BubikopfFacade::RespondTo(const std::string& moves)
{
    std::cout << "BubikopfFacade::RespondTo" << std::endl;
    std::vector<std::string> new_moves = SeparateEachMove(moves);
    CheckPlausibility(new_moves, last_moves_);
    const bool new_move_was_played = new_moves.size() == 1 + last_moves_.size();
    if (new_move_was_played)
    {
        last_moves_ = std::move(new_moves);
        CheckPlausibility(last_moves_.back());
        ConsiderTheirMove(last_moves_.back());
        return MakeMove();
    }
    else
    {
        std::cout << "Returning null move" << std::endl;
        return NULL_MOVE_;
    }
}

std::string BubikopfFacade::MakeMove()
{
    std::cout << "BubikopfFacade::MakeMove" << std::endl;
    populate(*node_, DEPTH_);
    minimax(*node_, DEPTH_, MIN_EVAL, MAX_EVAL);

    std::vector<NodePtr>::iterator chosen_move{};
    if (node_->position_.GetTurn() == Player::max)
    {
        chosen_move =
            std::max_element(node_->children_.begin(), node_->children_.end(), [](const auto& lhs, const auto& rhs) {
                return lhs->position_.evaluation_ > rhs->position_.evaluation_;
            });
    }
    else
    {
        chosen_move =
            std::min_element(node_->children_.begin(), node_->children_.end(), [](const auto& lhs, const auto& rhs) {
                return lhs->position_.evaluation_ < rhs->position_.evaluation_;
            });
    }

    if (chosen_move != node_->children_.end())
    {
        const std::size_t move_idx = static_cast<std::size_t>(chosen_move - node_->children_.begin());
        last_moves_.push_back(node_->children_.at(move_idx)->position_.previous_move_);
        node_ = ChooseChild(move_idx, std::move(node_));
    }
    else
    {
        std::cerr << "Error during MakeMove" << std::endl;
        throw std::string{"Error during MakeMove"};
    }

    std::cout << "Bubikopf plays " << last_moves_.back() << std::endl;
    std::cout << node_->position_.board_ << std::flush;

    return last_moves_.back();
}

void BubikopfFacade::ConsiderTheirMove(const std::string& move)
{
    std::cout << "BubikopfFacade::ConsiderTheirMove" << std::endl;
    for (std::size_t idx{0}; idx < node_->children_.size(); ++idx)
    {
        if (move == node_->children_.at(idx)->position_.previous_move_)
        {
            node_ = ChooseChild(idx, std::move(node_));
            break;
        }
    }

    std::cout << "They play " << move << std::endl;
    std::cout << node_->position_.board_ << std::flush;
}

}  // namespace Chess
