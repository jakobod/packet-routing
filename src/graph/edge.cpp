#include "graph/edge.hpp"

namespace graph {

edge::edge(int node_1, int node_2, int weight)
  : node_1(node_1), node_2(node_2), weight(weight) {
  // nop
}

bool edge::operator==(const edge& other) const {
  return ((node_1 == other.node_1) && (node_2 == other.node_2))
         || ((node_1 == other.node_2) && (node_2 == other.node_1));
}

bool edge::operator!=(const edge& other) const {
  return *this != other;
}

} // namespace graph
