#include "play/uci_interactor.h"

#include <iostream>
#include <iterator>
#include <sstream>

namespace Chess
{

UciInteractor::UciInteractor()
{
    log_file_.open("log.txt", std::ios::out | std::ios::app);
}

UciInteractor::~UciInteractor()
{
    log_file_.close();
}

void UciInteractor::Log(const std::string& message)
{
    const auto current_time = std::chrono::system_clock::to_time_t(std ::chrono::system_clock::now());
    const auto formatted_time = std::ctime(&current_time);

    log_file_ << formatted_time << message;
    if (message.back() == '\n')
    {
        log_file_ << std::endl;
    }
    else
    {
        log_file_ << '\n' << std::endl;
    }
}

void UciInteractor::ParseIncomingCommandsContinously()
{
    // Read new lines from std::cin in infinite loop
    for (std::string line; std::getline(std::cin, line);)
    {
        Log("Received: " + line);

        // Split line at spaces into separate tokens of command
        std::istringstream iss{line};
        const std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                              std::istream_iterator<std::string>{}};

        if (tokens.front() == "uci")
        {
            const std::string answer = "uciok\n";
            std::cout << answer;
            Log("Sent: " + answer);
            continue;
        }

        if (tokens.front() == "setoption")
        {
            // Do nothing. Configuration is done via config of lichess bot.
            Log("(no action & no response)");
            continue;
        }

        if (tokens.front() == "isready")
        {
            const std::string answer = "readyok\n";
            std::cout << answer;
            Log("Sent: " + answer);
            continue;
        }

        if (tokens.front() == "position" && tokens.back() == "startpos")
        {
            restart_game_.store(true);
            Log("Set: Restart game");
            continue;
        }

        if (tokens.front() == "position" && tokens.back() != "startpos")
        {
            // First three tokens are "position", "startpos" and "moves".
            std::vector<std::string> move_list{begin(tokens) + 3, end(tokens)};
            SetMoveList(std::move(move_list));
            Log("Set: (move list) " + line);
            continue;
        }

        if (tokens.front() == "go")
        {
            find_best_move_.store(true);
            Log("Set: Find best move");
            continue;
        }

        if (tokens.front() == "quit")
        {
            Log("Quitting");
            break;
        }

        const std::string unkown_command_error = "Error: Encountered unkown command";
        Log(unkown_command_error);
        throw std::runtime_error{unkown_command_error};
    }
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

}  // namespace Chess
