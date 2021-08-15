#define CAF_SUITE routing.ant

#include "caf/test/dsl.hpp"

#include "routing/ant.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() : params{1.0, 21.0, 3.0, 4.0, 5.0} {
    policy.init(seed_type{123456}, params);
    msg.update_path(3);
    msg.update_path(5);
    CAF_CHECK(msg.path_contains(3));
    CAF_CHECK(msg.path_contains(5));
  }

  routing::hyperparameters params;
  routing::ant policy;
  routing::message msg{1, 3, 2};
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(update) {
  policy.update(msg);
  CAF_CHECK_EQUAL(policy.get_route(id_type{3}), id_type{5});
}

CAF_TEST(delete_route) {
  policy.update(msg);
  CAF_CHECK_EQUAL(policy.get_route(3), id_type{5});
  policy.delete_route(id_type{3});
  CAF_CHECK_EQUAL(policy.get_route(id_type{3}),
                  std::numeric_limits<id_type>::max());
}

CAF_TEST_FIXTURE_SCOPE_END()
