#include "board/utilities.h"
#include "search/minimax.h"
#include "search/populate.h"

#include <algorithm>
#include <limits>

void PrintBoardWithPlyCaption(const Chess::Board& board, const int ply) {
  std::cout << "========= The board at ply: " << ply << " =========\n";
  std::cout << board;
}

void UserPause() {
  char command{'x'};
  while (command != 'c') {
    std::cout << "Press c to continue\n";
    std::cin >> command;
    std::cout << "You typed: " << command << '\n';
  }
  std::cout << "Continuing...\n";
}

int main() {
  Chess::NodePtr node{std::make_unique<Chess::Node>(Chess::SetUpBoard())};
  populate(*node, 1);
  Chess::Board board_after_human_move;

  while (true) {
    // Player moves
    std::cout << "\n\n\n\n\n\n";
    for (std::size_t idx{0}; idx < node->children_.size(); ++idx) {
      std::cout << "Option " << static_cast<int>(idx) << ":\n";
      std::cout << node->children_.at(idx)->state_.board_;
    }
    if (node->state_.plies_) {
      PrintBoardWithPlyCaption(board_after_human_move, node->state_.plies_ - 1);
    }
    PrintBoardWithPlyCaption(node->state_.board_, node->state_.plies_);
    std::size_t human_move_idx{std::numeric_limits<std::size_t>::max()};
    while (human_move_idx >= node->children_.size()) {
      std::cout << "Choose your move:\n";
      std::cin >> human_move_idx;
    }
    std::cout << "Option choosen: " << human_move_idx << '\n';
    node = Chess::ChooseChild(human_move_idx, std::move(node));
    UserPause();
    board_after_human_move = node->state_.board_;

    // AI moves
    const int DEPTH{5};
    populate(*node, DEPTH);
    std::vector<Chess::Evaluation> branch_evaluations{node->children_.size()};
    for (std::size_t idx{0}; idx < branch_evaluations.size(); idx++) {
      branch_evaluations.at(idx) = Chess::minimax(
          *(node->children_.at(idx)), DEPTH, Chess::MIN_EVAL, Chess::MAX_EVAL);
      std::cout << "Option " << static_cast<int>(idx) << " with evaluation of "
                << branch_evaluations.at(idx) << ":\n";
      std::cout << node->children_.at(idx)->state_.board_;
    }
    const auto min_element_itr =
        std::min_element(branch_evaluations.begin(), branch_evaluations.end());
    if (min_element_itr != branch_evaluations.end()) {
      std::size_t ai_move_idx = static_cast<std::size_t>(
          min_element_itr - branch_evaluations.begin());
      std::cout << "Ai's choice with evaluation of "
                << branch_evaluations.at(ai_move_idx) << ":\n";
      std::cout << node->children_.at(ai_move_idx)->state_.board_;
      node = Chess::ChooseChild(ai_move_idx, std::move(node));
      std::cout << "Number of nodes: " << Chess::CountNodes(*node) << '\n';
      UserPause();
    } else {
      throw - 1;
    }
  }

  return 0;
}
