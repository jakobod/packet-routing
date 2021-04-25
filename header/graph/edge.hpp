#pragma once

#include <cstddef>
#include <functional>

namespace graph {

/// An edge of an undirected graph.
struct edge {
  edge(int node_1, int node_2, int weight);
  bool operator==(const edge& other) const;
  bool operator!=(const edge& other) const;

  int node_1;
  int node_2;
  int weight;
};

} // namespace graph

namespace std {

template <>
struct hash<::graph::edge> {
  std::size_t operator()(const ::graph::edge& e) const noexcept {
    auto h1 = std::hash<int>{}(e.node_1);
    auto h2 = std::hash<int>{}(e.node_2);
    auto h3 = std::hash<int>{}(e.weight);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

} // namespace std
