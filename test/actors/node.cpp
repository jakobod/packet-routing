#define CAF_SUITE actors.node

#include "caf/test/dsl.hpp"

#include "actors/node.hpp"

#include "actors/transition.hpp"
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

CAF_TEST(remove transition kill of other node) {
  routing::hyperparameters params{1.0, 2.0, 3.0, 4.0, 5.0};
  CAF_MESSAGE("Spawning two nodes and a transition");
  auto node_1 = sys.spawn(actors::node, 1, 1, self, self, params, true);
  auto node_1_ptr = actor_cast<stateful_actor<actors::node_state>*>(node_1);
  CAF_CHECK(node_1_ptr);
  auto node_2 = sys.spawn(actors::node, 2, 1, self, self, params, true);
  auto trans = sys.spawn(actors::transition, std::make_pair(node_1, 1),
                         std::make_pair(node_2, 2), self, 1, self);
  run();
  expect((done_atom), from(trans).to(self).with(_));
  CAF_CHECK_EQUAL(node_1_ptr->state.transitions.size(), size_t{1});
  CAF_MESSAGE("Kill other node");
  self->send_exit(node_2, exit_reason::kill);
  run();
  CAF_CHECK(node_1_ptr->state.transitions.empty());
  // Ensure that transition does NOT link to parent
  disallow((exit_msg), from(trans).to(self).with(_));
}

CAF_TEST(remove transition on kill of transition) {
  routing::hyperparameters params{1.0, 2.0, 3.0, 4.0, 5.0};
  CAF_MESSAGE("Spawning two nodes and a transition");
  auto node_1 = sys.spawn(actors::node, 1, 1, self, self, params, true);
  auto node_1_ptr = actor_cast<stateful_actor<actors::node_state>*>(node_1);
  CAF_CHECK(node_1_ptr);
  auto node_2 = sys.spawn(actors::node, 2, 1, self, self, params, true);
  auto node_2_ptr = actor_cast<stateful_actor<actors::node_state>*>(node_1);
  CAF_CHECK(node_2_ptr);
  auto trans = sys.spawn(actors::transition, std::make_pair(node_1, 1),
                         std::make_pair(node_2, 2), self, 1, self);
  run();
  expect((done_atom), from(trans).to(self).with(_));
  CAF_CHECK_EQUAL(node_1_ptr->state.transitions.size(), size_t{1});
  CAF_CHECK_EQUAL(node_2_ptr->state.transitions.size(), size_t{1});
  CAF_MESSAGE("Kill transition");
  self->send_exit(trans, exit_reason::kill);
  run();
  CAF_CHECK(node_1_ptr->state.transitions.empty());
  CAF_CHECK(node_2_ptr->state.transitions.empty());
  // Ensure that transition does NOT link to parent
  disallow((exit_msg), from(trans).to(self).with(_));
}

CAF_TEST_FIXTURE_SCOPE_END()
