/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file transition.cpp
 */

#include "graph/transition.hpp"

namespace graph {

transition::transition(id_type node_1, id_type node_2, weight_type weight)
  : node_1(node_1), node_2(node_2), weight(weight) {
  // nop
}

bool transition::operator==(const transition& other) const {
  return ((node_1 == other.node_1) && (node_2 == other.node_2))
         || ((node_1 == other.node_2) && (node_2 == other.node_1));
}

bool transition::operator!=(const transition& other) const {
  return !(*this == other);
}

} // namespace graph
