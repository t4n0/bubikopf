#include "play/bubikopf.h"

#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "evaluate/evaluate.h"
#include "search/find_best_move.h"

#include <algorithm>

namespace Chess
{

void Bubikopf::RestartGame()
{
    position_ = Position{PositionFromFen(kStandardStartingPosition)};
}

void Bubikopf::UpdateBoard(const std::vector<std::string>& move_list)
{
    const auto played_plies_internal = position_.GetNumberOfPlayedPlies();
    if (move_list.size() < played_plies_internal)
    {
        throw std::runtime_error{"Move list from gui is behind engine."};
    }

    for (std::size_t new_move = played_plies_internal; new_move < move_list.size(); new_move++)
    {
        const std::string& new_move_uci = move_list.at(new_move);
        const auto possible_moves_end = GenerateMoves<GenerateAllPseudoLegalMoves>(position_, begin(move_list_));

        const auto move_to_play =
            std::find_if(begin(move_list_), possible_moves_end, [&new_move_uci](const auto& move) {
                return new_move_uci == ToUciString(move);
            });

        if (move_to_play == possible_moves_end)
        {
            throw std::runtime_error{"Move played by gui not possible from internal representation."};
        }

        position_.MakeMove(*move_to_play);
    }
}

std::string Bubikopf::FindBestMove()
{
    constexpr int depth = 6;
    Bitmove best_move;
    Evaluation evaluation;
    std::tie(best_move, evaluation) =
        Chess::FindBestMove<GenerateAllPseudoLegalMoves, EvaluteMaterial>(depth, position_, begin(move_list_));
    return ToUciString(best_move);
}

}  // namespace Chess
