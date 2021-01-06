#include "play/bubikopf.h"
#include "play/logging.h"
#include "play/uci_interactor.h"

#include <thread>

int main()
{
    try
    {
        Chess::Bubikopf engine_api{};
        Chess::UciInteractor uci_interactor{};

        std::thread uci_interaction{&Chess::UciInteractor::ParseIncomingCommandsContinously, std::ref(uci_interactor)};

        while (!uci_interactor.quit_game_.load())
        {
            if (uci_interactor.restart_game_.load())
            {
                uci_interactor.restart_game_.store(false);
                engine_api.SetUpBoardInStandardStartingPosition();
            }

            if (uci_interactor.find_best_move_.load())
            {
                uci_interactor.find_best_move_.store(false);
                engine_api.UpdateBoard(uci_interactor.GetMoveList());
                const auto [best_move, game_result] = engine_api.FindBestMove();
                uci_interactor.SendBestMoveOnce(best_move);
            }
        }

        uci_interaction.join();
    }
    catch (const std::exception& e)
    {
        const auto error_message = std::string{"Exception caught in main: "} + e.what();
        Chess::ToCerrWithTime(error_message);
    }

    return 0;
}
