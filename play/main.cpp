#include "play/bubikopf.h"
#include "play/uci_interactor.h"

#include <thread>

int main()
{
    Chess::UciInteractor uci_interactor{};
    Chess::Bubikopf engine_api{};

    std::thread uci_interaction{&Chess::UciInteractor::ParseIncomingCommandsContinously, std::ref(uci_interactor)};

    while (!uci_interactor.quit_game_.load())
    {
        if (uci_interactor.restart_game_.load())
        {
            engine_api.RestartGame();
        }

        if (uci_interactor.find_best_move_.load())
        {
            engine_api.UpdateBoard(uci_interactor.GetMoveList());
            const auto [best_move, game_result] = engine_api.FindBestMove();
            uci_interactor.SendBestMoveOnce(best_move);
        }
    }

    uci_interaction.join();

    return 0;
}
