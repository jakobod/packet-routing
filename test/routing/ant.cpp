#define CAF_SUITE routing.ant

#include "caf/test/dsl.hpp"

#include "routing/ant.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() : params{1.0, 21.0, 3.0, 4.0, 5.0} {
    policy.init(seed_type{123456}, params);
  }

  routing::hyperparameters params;
  routing::ant policy;
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(update) {
  routing::message msg{1, 1, 3};
  msg.update_path(2);
  policy.update(msg);
  CAF_CHECK(policy.get_route(3), 2);
}

CAF_TEST(delete_route) {
  routing::message msg{1, 1, 3};
  msg.update_path(2);
  policy.update(msg);
  CAF_CHECK(policy.get_route(3), 2);
  policy.delete_route(3);
  CAF_CHECK(policy.get_route(3), std::numeric_limits<id_type>::max());
}

CAF_TEST_FIXTURE_SCOPE_END()
