/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file transition.hpp
 */

#pragma once

#include <cstddef>
#include <functional>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "types.hpp"

namespace graph {

/// An transition of an undirected graph.
struct transition {
  transition(id_type node_1, id_type node_2, weight_type weight, bool alive);

  // -- Equality operators -----------------------------------------------------

  bool operator==(const transition& other) const;

  bool operator!=(const transition& other) const;

  bool connects_same_node() const;

  // -- Stream operators -------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& os, const transition& t) {
    std::stringstream ss;
    ss << t.node_1 << " <" << t.weight << "> " << t.node_2;
    return os << ss.str();
  }

  // -- public members ---------------------------------------------------------
  id_type node_1;
  id_type node_2;
  id_type weight;
  bool alive;
};

using transition_list = std::vector<transition>;
using transition_set = std::unordered_set<transition>;

} // namespace graph

namespace std {

template <>
struct hash<::graph::transition> {
  std::size_t operator()(const ::graph::transition& t) const noexcept {
    auto h1 = std::hash<id_type>{}(t.node_1);
    auto h2 = std::hash<id_type>{}(t.node_2);
    return h1 ^ (h2 << 1);
  }
};

} // namespace std
