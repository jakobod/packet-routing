#define CAF_SUITE transition

#include "caf/test/dsl.hpp"

#include "actors/transition.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(graph_tests, fixture)

CAF_TEST() {
  // scoped_actor node_1;
  // scoped_actor node_2;
  // auto trans = sys.spawn(actors::transition, );
}

CAF_TEST_FIXTURE_SCOPE_END()
