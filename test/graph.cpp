#define CAF_SUITE graph

#include "caf/test/dsl.hpp"

#include "graph/generator.hpp"

using namespace caf;

namespace {

struct fixture {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(graph_tests, fixture)

CAF_TEST(Graph Test) {
  auto g = graph::generate_random_graph(10, 18, 0);
  graph::log_graph(g);
  auto verteces = graph::get_verteces(g);
  auto edges = graph::get_edges(g);
  CAF_CHECK_EQUAL(size_t(10), verteces.size());
  CAF_CHECK_EQUAL(size_t(18), edges.size());
}

CAF_TEST_FIXTURE_SCOPE_END()
