#define CAF_SUITE routing.entry

#include "caf/test/dsl.hpp"

#include "routing/entry.hpp"
#include "routing/hyperparameters.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() : params{1.0, 1.0, 1.0, 1.0, 1.0}, entry{id_type{1}, params, 42} {
    // nop
  }

  routing::hyperparameters params;
  routing::entry entry;

  // Test data
  std::array<weight_type, 10> update_data{1,   10,   100,   1000,   69,
                                          420, 1337, 69420, 574833, 1284842375};

  // std::array<std::pair<routing::hyperparameters, double>, 5> value_data {
  //   std::make_pair(routing::hyperparameters{}, 1);
  // };
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(initialization) {
  CAF_CHECK_EQUAL(entry.pheromones, 1.0);
  CAF_CHECK_EQUAL(entry.weight, weight_type(42));
  CAF_CHECK_EQUAL(entry.pheromone_deposition, params.pheromone_deposition);
  CAF_CHECK_EQUAL(entry.pheromone_evaporation, params.pheromone_evaporation);
  CAF_CHECK_EQUAL(entry.beta, params.beta);
  CAF_CHECK_EQUAL(entry.alpha, params.alpha);
  CAF_CHECK_EQUAL(entry.next_hop, id_type{1});
}

CAF_TEST(update) {
  for (size_t i = 0; i < update_data.size(); ++i) {
    entry.update(update_data[i]);
    CAF_CHECK_EQUAL(entry.weight, update_data[i]);
    CAF_CHECK_EQUAL(entry.pheromones,
                    params.pheromone_deposition
                      + ((i + 1) * params.pheromone_deposition));
  }
}

CAF_TEST_FIXTURE_SCOPE_END()
