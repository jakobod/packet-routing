#define CAF_SUITE graph.generator

#include "caf/test/dsl.hpp"

#include <unordered_set>

#include "graph/generator.hpp"

CAF_TEST(Generate graph) {
  const size_t num_nodes = 10;
  const size_t num_transitions = 18;
  const size_t seed = 0;
  auto g = graph::generate_random_graph(num_nodes, num_transitions, seed);
  auto nodes = graph::get_nodes(g);
  auto transitions = graph::get_transitions(g);
  CAF_MESSAGE("Check nodes");
  CAF_CHECK_EQUAL(num_nodes, nodes.size());
  std::unordered_set<graph::node> node_set{nodes.begin(), nodes.end()};
  CAF_CHECK_EQUAL(nodes.size(), node_set.size());
  CAF_MESSAGE("Check transitions");
  CAF_CHECK_EQUAL(num_transitions, transitions.size());
  std::unordered_set<graph::transition> transition_set{transitions.begin(),
                                                       transitions.end()};
  CAF_CHECK_EQUAL(transitions.size(), transition_set.size());
}
