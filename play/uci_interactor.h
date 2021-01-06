#ifndef PLAY_UCI_INTERACTOR_H
#define PLAY_UCI_INTERACTOR_H

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

namespace Chess
{

class UciInteractor
{
  public:
    void ParseIncomingCommandsContinously();
    void SendBestMoveOnce(const std::string& move);
    std::vector<std::string> GetMoveList();

    std::atomic_bool quit_game_{false};
    std::atomic_bool restart_game_{false};
    std::atomic_bool find_best_move_{false};

  private:
    void SetMoveList(std::vector<std::string>&& move_list);

    /// @brief Writes thread-safe to cout.
    void ToCout(const std::string& command);

    std::vector<std::string> move_list_{};
    std::mutex move_list_mutex_{};
};

}  // namespace Chess

#endif
