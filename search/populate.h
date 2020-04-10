#ifndef SEARCH_POPULATE_H
#define CHESS_POPULATE_H

#include "evaluate/evaluation.h"
#include "search/node.h"

#include <memory>
#include <vector>

namespace Chess {

void populate(Node& node, const int depth);

}  // namespace Chess

#endif
