#define CAF_SUITE graph.generator

#include "caf/test/dsl.hpp"

#include <unordered_set>

#include "graph/generator.hpp"

CAF_TEST(Generate graph) {
  const size_t num_verteces = 10;
  const size_t num_edges = 18;
  const size_t seed = 0;
  auto g = graph::generate_random_graph(num_verteces, num_edges, seed);
  auto verteces = graph::get_verteces(g);
  auto edges = graph::get_edges(g);
  CAF_MESSAGE("Check vertex list");
  CAF_CHECK_EQUAL(num_verteces, verteces.size());
  std::unordered_set<graph::vertex> vertex_set{verteces.begin(),
                                               verteces.end()};
  CAF_CHECK_EQUAL(verteces.size(), vertex_set.size());
  CAF_MESSAGE("Check edge list");
  CAF_CHECK_EQUAL(num_edges, edges.size());
  std::unordered_set<graph::edge> edge_set{edges.begin(), edges.end()};
  CAF_CHECK_EQUAL(edges.size(), edge_set.size());
}
