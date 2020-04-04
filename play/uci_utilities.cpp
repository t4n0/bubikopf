#include "play/uci_utilities.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

namespace Chess {

std::vector<std::string> SeparateEachMove(const std::string& moves_uci) {
  std::vector<std::string> moves{};
  if (!moves_uci.empty()) {
    boost::algorithm::split(moves, moves_uci, boost::is_any_of(" "),
                            boost::token_compress_on);
  }
  return moves;
}

}  // namespace Chess
