#include "play/logging.h"

#include <chrono>
#include <iostream>

namespace Chess
{

void ToCerrWithTime(const std::string& message)
{
    const auto current_time = std::chrono::system_clock::to_time_t(std::chrono::high_resolution_clock::now());
    auto formatted_time = std::string{std::ctime(&current_time)};
    formatted_time.pop_back();  // remove trailing '\n'

    std::string time_and_message = formatted_time + ": " + message;
    if (time_and_message.back() != '\n')
    {
        time_and_message += '\n';
    }

    std::cerr << time_and_message << std::flush;  // Mangling here is not harmfull. I.e. two threads could end up doing:
                                                  // "MessageA\nMessageB\n" and flushing twice afterwards.
}

}  // namespace Chess
