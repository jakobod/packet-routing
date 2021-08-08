#define CAF_SUITE actors.transition

#include "caf/test/dsl.hpp"

#include <utility>

#include "actors/transition.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace {

constexpr int node_1_id = 1;
constexpr int node_2_id = 2;
constexpr int trans_weight = 1;

struct fixture : test_coordinator_fixture<> {
  fixture()
    : node_1{sys},
      node_2{sys},
      parent{sys},
      listener{sys},
      trans{sys.spawn(actors::transition_actor,
                      std::make_pair(actor_cast<actor>(node_1), node_1_id),
                      std::make_pair(actor_cast<actor>(node_2), node_2_id),
                      parent, trans_weight, listener)} {
    // nop
  }

  void skip_register_messages() {
    run();
    node_1->receive([](register_transition_atom, actor, int) {});
    node_2->receive([](register_transition_atom, actor, int) {});
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
  expect((register_transition_atom, actor, int),
         from(trans).to(node_1).with(_, _, node_2_id));
  expect((register_transition_atom, actor, int),
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

CAF_TEST_FIXTURE_SCOPE_END()
