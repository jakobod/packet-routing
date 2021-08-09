/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file transition.hpp
 */

#pragma once

#include <cstddef>
#include <functional>

#include "types.hpp"

namespace graph {

/// An transition of an undirected graph.
struct transition {
  transition(id_type node_1, id_type node_2, weight_type weight);
  bool operator==(const transition& other) const;
  bool operator!=(const transition& other) const;

  id_type node_1;
  id_type node_2;
  id_type weight;
};

} // namespace graph

namespace std {

template <>
struct hash<::graph::transition> {
  std::size_t operator()(const ::graph::transition& t) const noexcept {
    auto h1 = std::hash<id_type>{}(t.node_1);
    auto h2 = std::hash<id_type>{}(t.node_2);
    auto h3 = std::hash<id_type>{}(t.weight);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

} // namespace std
