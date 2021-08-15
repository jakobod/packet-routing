/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file node.hpp
 */

#pragma once

#include <unordered_set>
#include <vector>

#include "types.hpp"

namespace graph {

// -- type declarations --------------------------------------------------------

using node = id_type;

using node_list = std::vector<node>;

using node_set = std::unordered_set<node>;

} // namespace graph
