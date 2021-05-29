#define CAF_SUITE generator

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

CAF_TEST_FIXTURE_SCOPE(ip_tests, fixture)

CAF_TEST(Graph Test) {
  auto g = graph::generate_random_graph(10, 18, 0);
  graph::log_graph(g);
  auto verteces = graph::get_verteces(g);
  CAF_CHECK_EQUAL(size_t(10), verteces.size());
}

CAF_TEST_FIXTURE_SCOPE_END()
