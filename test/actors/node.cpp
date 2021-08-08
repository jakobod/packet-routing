#define CAF_SUITE actors.node

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

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(something) {
  // nop
}

CAF_TEST_FIXTURE_SCOPE_END()
