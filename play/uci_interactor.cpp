#include "play/uci_interactor.h"

#include "play/logging.h"

#include <iostream>
#include <iterator>
#include <sstream>

namespace Chess
{

void UciInteractor::ParseIncomingCommandsContinously()
{
    // Read new lines from std::cin in infinite loop
    for (std::string line; std::getline(std::cin, line);)
    {
        ToCerrWithTime("Received: " + line);

        // Split line at spaces into separate tokens of command
        std::istringstream iss{line};
        const std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                              std::istream_iterator<std::string>{}};

        if (tokens.front() == "uci")
        {
            ToCout("uciok");
            continue;
        }

        if (tokens.front() == "setoption")
        {
            // Do nothing. Configuration is done via config of lichess bot.
            ToCerrWithTime("noop");
            continue;
        }

        if (tokens.front() == "isready")
        {
            ToCout("readyok");
            continue;
        }

        if (tokens.front() == "position" && tokens.back() == "startpos")
        {
            restart_game_.store(true);
            ToCerrWithTime("Set: Restart game");
            continue;
        }

        if (tokens.front() == "position" && tokens.back() != "startpos")
        {
            // First three tokens are "position", "startpos" and "moves".
            std::vector<std::string> move_list{begin(tokens) + 3, end(tokens)};
            SetMoveList(std::move(move_list));
            ToCerrWithTime("Set: (move list) " + line);
            continue;
        }

        if (tokens.front() == "go")
        {
            find_best_move_.store(true);
            ToCerrWithTime("Set: Go");
            continue;
        }

        if (tokens.front() == "quit")
        {
            quit_game_.store(true);
            ToCerrWithTime("Set: Quit");
            break;
        }

        const std::string unkown_command_error = "Error. Encountered unkown command: \"" + line + "\"";
        ToCerrWithTime(unkown_command_error);
        throw std::runtime_error{unkown_command_error};
    }
}

void UciInteractor::SendBestMoveOnce(const std::string& move)
{
    ToCout("bestmove " + move);
}

std::vector<std::string> UciInteractor::GetMoveList()
{
    const std::lock_guard<std::mutex> move_list_guard{move_list_mutex_};
    return move_list_;
}

void UciInteractor::SetMoveList(std::vector<std::string>&& move_list)
{
    const std::lock_guard<std::mutex> move_list_guard{move_list_mutex_};
    move_list_.swap(move_list);
}

void UciInteractor::ToCout(const std::string& command)
{
    const std::string answer = command + '\n';
    std::cout << answer << std::flush;  // Mangling here is not harmfull. I.e. two threads could end up doing:
                                        // "CommandA\nCommandB\n" and flushing twice afterwards.
    ToCerrWithTime("Sent: " + answer);
}

}  // namespace Chess
