#ifndef PLAY_UCI_INTERACTOR_H
#define PLAY_UCI_INTERACTOR_H

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

namespace Chess
{

class UciInteractor
{
  public:
    UciInteractor();
    ~UciInteractor();

    void ParseIncomingCommandsContinously();

    std::vector<std::string> GetMoveList();
    std::atomic_bool restart_game_{false};
    std::atomic_bool find_best_move_{false};

  private:
    void SetMoveList(std::vector<std::string>&& move_list);
    void Log(const std::string& message);

    std::vector<std::string> move_list_{};
    std::mutex move_list_mutex_{};
    std::ofstream log_file_;
};

}  // namespace Chess

#endif
