/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file edge.cpp
 */

#include "graph/edge.hpp"

namespace graph {

edge::edge(id_type node_1, id_type node_2, weight_type weight)
  : node_1(node_1), node_2(node_2), weight(weight) {
  // nop
}

bool edge::operator==(const edge& other) const {
  return ((node_1 == other.node_1) && (node_2 == other.node_2))
         || ((node_1 == other.node_2) && (node_2 == other.node_1));
}

bool edge::operator!=(const edge& other) const {
  return !(*this == other);
}

} // namespace graph
