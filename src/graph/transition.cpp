/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file transition.cpp
 */

#include "graph/transition.hpp"

namespace graph {

transition::transition(id_type node_1, id_type node_2, weight_type weight,
                       bool alive)
  : node_1(std::min(node_1, node_2)),
    node_2(std::max(node_1, node_2)),
    weight(weight),
    alive(alive) {
  // nop
}

bool transition::operator==(const transition& other) const {
  return ((node_1 == other.node_1) && (node_2 == other.node_2));
}

bool transition::operator!=(const transition& other) const {
  return !(*this == other);
}

bool transition::connects_same_node() const {
  return node_1 == node_2;
}

} // namespace graph
