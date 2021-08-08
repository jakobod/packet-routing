#define CAF_SUITE routing.random

#include "caf/test/dsl.hpp"

#include "routing/random.hpp"

#include <memory>

CAF_TEST(Get Route) {
  routing::random policy;
  CAF_CHECK_EQUAL(policy.get_route(0), std::numeric_limits<id_type>::max());
}

// Hunting a memory problem... Otherwise this test is nonesense.
CAF_TEST(Get Route with shared_ptr) {
  std::shared_ptr<routing::policy> policy{std::make_shared<routing::random>()};
  CAF_CHECK_EQUAL(policy->get_route(0), std::numeric_limits<id_type>::max());
}
