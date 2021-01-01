#include "play/logger.h"

namespace Chess
{

Logger::Logger(const std::string& filename)
{
    log_file_.open(filename, std::ios::out | std::ios::app);
    if (!log_file_.is_open())
    {
        throw std::runtime_error{"Setting up logging failed."};
    }
}

Logger::~Logger()
{
    log_file_.close();
}

void Logger::Log(const std::string& message)
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

}  // namespace Chess
