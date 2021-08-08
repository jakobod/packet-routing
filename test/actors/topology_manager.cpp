#define CAF_SUITE actors.topology_manager

#include "caf/test/dsl.hpp"

#include <utility>

#include "actors/node.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(topology_manager_tests, fixture)

CAF_TEST(initialize) {
  // nop
}

CAF_TEST(forward message) {
  // nop
}

CAF_TEST_FIXTURE_SCOPE_END()
