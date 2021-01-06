#ifndef PLAY_LOGGING_H
#define PLAY_LOGGING_H

#include <string>

namespace Chess
{

/// @brief Writes thread-safe to cerr and prepends current time.
void ToCerrWithTime(const std::string& message);

}  // namespace Chess

#endif
