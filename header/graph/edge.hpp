/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file edge.hpp
 */

#pragma once

#include <cstddef>
#include <functional>

#include "types.hpp"

namespace graph {

/// An edge of an undirected graph.
struct edge {
  edge(id_type node_1, id_type node_2, weight_type weight);
  bool operator==(const edge& other) const;
  bool operator!=(const edge& other) const;

  id_type node_1;
  id_type node_2;
  id_type weight;
};

} // namespace graph

namespace std {

template <>
struct hash<::graph::edge> {
  std::size_t operator()(const ::graph::edge& e) const noexcept {
    auto h1 = std::hash<id_type>{}(e.node_1);
    auto h2 = std::hash<id_type>{}(e.node_2);
    auto h3 = std::hash<id_type>{}(e.weight);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

} // namespace std
