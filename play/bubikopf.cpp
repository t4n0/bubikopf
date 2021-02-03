#include "play/bubikopf.h"

#include "bitboard/fen_conversion.h"
#include "bitboard/generate_moves.h"
#include "bitboard/uci_conversion.h"
#include "evaluate/evaluate.h"
#include "play/logging.h"
#include "search/find_best_move.h"

#include <algorithm>

namespace Chess
{

Bubikopf::Bubikopf()
{
    SetUpBoardInStandardStartingPosition();
}

void Bubikopf::SetUpBoardAccordingToFen(const std::string& fen)
{
    position_ = PositionFromFen(fen);
    ToCerrWithTime("Set up fen: " + fen);
}

void Bubikopf::SetUpBoardInStandardStartingPosition()
{
    position_ = PositionFromFen(kStandardStartingPosition);
    ToCerrWithTime("Set up standard position.");
}

void Bubikopf::UpdateBoard(const std::vector<std::string>& move_list)
{
    const auto played_plies_internal = position_.GetTotalPlies();
    if (move_list.size() < played_plies_internal)
    {
        const std::string message = "Move list from gui is behind engine. Gui: " + std::to_string(move_list.size()) +
                                    ", Engine: " + std::to_string(played_plies_internal);
        ToCerrWithTime(message);
        throw std::runtime_error{message};
    }

    for (std::size_t new_move = played_plies_internal; new_move < move_list.size(); new_move++)
    {
        const std::string& new_move_uci = move_list[new_move];
        const auto possible_moves_end = GenerateMoves<GenerateAllPseudoLegalMoves>(position_, begin(move_stack_));

        const auto move_to_play =
            std::find_if(begin(move_stack_), possible_moves_end, [&new_move_uci](const auto& move) {
                return new_move_uci == ToUciString(move);
            });

        if (move_to_play == possible_moves_end)
        {
            constexpr auto message = "Move played by gui not possible from internal representation.";
            ToCerrWithTime(message);
            throw std::runtime_error{message};
        }

        ToCerrWithTime("Playing " + ToUciString(*move_to_play));
        std::ignore = position_.MakeMove(*move_to_play);
    }
}

std::tuple<std::string, Evaluation> Bubikopf::FindBestMove()
{
    ToCerrWithTime("Starting search for best move.");
    constexpr std::size_t full_search_depth = 6;
    constexpr AbortCondition abort_condition{full_search_depth};
    const auto [best_move, evaluation] = Chess::FindBestMove<GenerateAllPseudoLegalMoves, EvaluateMaterial>(
        position_, begin(move_stack_), GetCurrentNegamaxSign(), abort_condition);
    const auto uci_move = ToUciString(best_move);
    ToCerrWithTime("Best move is: " + uci_move);
    return {uci_move, evaluation * GetCurrentNegamaxSign()};
}

void Bubikopf::PrintBoard() const
{
    PrettyPrintFen(FenFromPosition(position_));
}

Evaluation Bubikopf::GetCurrentNegamaxSign() const
{
    return position_.white_to_move_ ? Evaluation{1} : Evaluation{-1};
}

}  // namespace Chess
