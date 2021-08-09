#define CAF_SUITE routing.entry

#include "caf/test/dsl.hpp"

#include "routing/entry.hpp"
#include "routing/hyperparameters.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(initialization) {
  routing::hyperparameters params{1.0, 2.0, 3.0, 4.0, 5.0};
  routing::entry e{id_type{1}, params};

  CAF_CHECK_EQUAL(e.pheromones, double{0});
  CAF_CHECK_EQUAL(e.weight, weight_type(0));
  CAF_CHECK_EQUAL(e.pheromone_deposition, params.pheromone_deposition);
  CAF_CHECK_EQUAL(e.pheromone_evaporation, params.pheromone_evaporation);
  CAF_CHECK_EQUAL(e.beta, params.beta);
  CAF_CHECK_EQUAL(e.alpha, params.alpha);
  CAF_CHECK_EQUAL(e.next_hop, id_type{1});
}

CAF_TEST_FIXTURE_SCOPE_END()
