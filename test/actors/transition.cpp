#define CAF_SUITE actors.transition

#include "actors/transition.hpp"

#include "caf/test/dsl.hpp"

#include <utility>

#include "actors/node.hpp"
#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"
#include "types.hpp"

using namespace caf;

namespace {

constexpr id_type node_1_id = 1;
constexpr id_type node_2_id = 2;
constexpr weight_type trans_weight = 1;

struct fixture : test_coordinator_fixture<> {
  fixture()
    : node_1{sys},
      node_2{sys},
      parent{sys},
      listener{sys},
      trans{sys.spawn(actors::transition,
                      std::make_pair(actor_cast<actor>(node_1), node_1_id),
                      std::make_pair(actor_cast<actor>(node_2), node_2_id),
                      parent, trans_weight, listener)} {
    // nop
  }

  void skip_register_messages() {
    run();
    node_1->receive([](register_transition_atom, actor, id_type) {});
    node_2->receive([](register_transition_atom, actor, id_type) {});
  }

  scoped_actor node_1;
  scoped_actor node_2;
  scoped_actor parent;
  scoped_actor listener;
  actor trans;
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(transition_tests, fixture)

CAF_TEST(initialize) {
  CAF_MESSAGE("Expecting the transition to register itself");
  run();
  expect((register_transition_atom, actor, id_type),
         from(trans).to(node_1).with(_, _, node_2_id));
  expect((register_transition_atom, actor, id_type),
         from(trans).to(node_2).with(_, _, node_1_id));
  CAF_MESSAGE("Answering with done_atoms");
  node_1->send(trans, done_atom_v);
  node_2->send(trans, done_atom_v);
  CAF_MESSAGE("Expecting notification from transition");
  run();
  expect((done_atom), from(trans).to(parent).with(done_atom_v));
}

CAF_TEST(forward message) {
  using namespace std::chrono_literals;
  skip_register_messages();
  CAF_MESSAGE("Sending messages to transition to forward");
  node_1->send(trans, message_atom_v, routing::message{});
  node_2->send(trans, message_atom_v, routing::message{});
  CAF_MESSAGE("Expecting the messages at the other node");
  run();
  expect((message_atom, routing::message), from(trans).to(node_2).with(_, _));
  expect((message_atom, routing::message), from(trans).to(node_1).with(_, _));
}

CAF_TEST(quit on down_msg) {
  routing::hyperparameters params{1.0, 2.0, 3.0, 4.0, 5.0};
  CAF_MESSAGE("Spawning two nodes and a transition");
  auto node_1 = sys.spawn(actors::node, 1, 1, self, self, params, true);
  auto node_2 = sys.spawn(actors::node, 2, 1, self, self, params, true);
  auto trans = sys.spawn(actors::transition, std::make_pair(node_1, 1),
                         std::make_pair(node_2, 2), self, 1, self);
  self->monitor(trans);
  run();
  expect((done_atom), from(trans).to(self).with(_));
  self->send_exit(node_1, exit_reason::kill);
  run();
  expect((down_msg), from(trans).to(self).with(_));
}

CAF_TEST_FIXTURE_SCOPE_END()
