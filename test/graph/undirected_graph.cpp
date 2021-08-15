#define CAF_SUITE graph.undirected_graph

#include "caf/test/dsl.hpp"

#include "graph/undirected_graph.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(undirected_graph_tests, fixture)

CAF_TEST(number of transitions) {
  CAF_CHECK_EQUAL(graph::generate_random_undirected_graph(1, 100, 1),
                  std::nullopt);
  CAF_CHECK_EQUAL(graph::generate_random_undirected_graph(10, 1000, 1),
                  std::nullopt);
  CAF_CHECK_EQUAL(graph::generate_random_undirected_graph(643, 206404, 1),
                  std::nullopt);
  CAF_CHECK_NOT_EQUAL(graph::generate_random_undirected_graph(2, 1, 1),
                      std::nullopt);
  CAF_CHECK_NOT_EQUAL(graph::generate_random_undirected_graph(10, 20, 1),
                      std::nullopt);
  CAF_CHECK_NOT_EQUAL(graph::generate_random_undirected_graph(643, 2246, 1),
                      std::nullopt);
}

CAF_TEST(No duplicates) {
  auto res = graph::generate_random_undirected_graph(10, 20, 1);
  CAF_CHECK_NOT_EQUAL(res, std::nullopt);
  auto g = *res;
  std::stringstream ss;
  ss << g;
  CAF_MESSAGE(ss.str());
  auto& transitions = g.transitions();
  graph::transition_set trans_set(transitions.begin(), transitions.end());
  CAF_CHECK_EQUAL(trans_set.size(), transitions.size());
  auto& nodes = g.nodes();
  graph::node_set node_set(nodes.begin(), nodes.end());
  CAF_CHECK_EQUAL(node_set.size(), nodes.size());
}

CAF_TEST_FIXTURE_SCOPE_END()
