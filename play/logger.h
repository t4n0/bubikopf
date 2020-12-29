#ifndef PLAY_LOGGER_H
#define PLAY_LOGGER_H

#include <chrono>
#include <fstream>

namespace Chess
{

class Logger
{
  public:
    Logger();
    ~Logger();

    void Log(const std::string& message);

  private:
    std::ofstream log_file_;
};

}  // namespace Chess

#endif
