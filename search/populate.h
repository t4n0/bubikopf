#ifndef SEARCH_POPULATE_H
#define CHESS_POPULATE_H

#include "search/evaluation.h"
#include "search/node.h"

#include <memory>
#include <vector>

namespace Chess {

using NodePtr = std::unique_ptr<Node>;

std::vector<NodePtr> collect_plies_from_all_pieces(const Node&);

int CountChildren(const Node& node);

void populate(Node& node, const int depth);

}  // namespace Chess

#endif
