#ifndef PLAY_BUBIKOPF_H
#define PLAY_BUBIKOPF_H

#include "bitboard/move_stack.h"
#include "bitboard/position.h"
#include "search/principal_variation.h"

#include <string>
#include <vector>

namespace Chess
{

class Bubikopf
{
  public:
    Bubikopf();
    void SetUpBoardInStandardStartingPosition();
    void SetUpBoardAccordingToFen(const std::string& fen);
    void UpdateBoard(const std::vector<std::string>& move_list);
    std::tuple<std::string, Evaluation> FindBestMove();
    void PrintBoard() const;

  private:
    Evaluation GetCurrentNegamaxSign() const;

    Position position_{};
    MoveStack move_stack_{};
    PrincipalVariation principal_variation_{};
};

}  // namespace Chess

#endif
