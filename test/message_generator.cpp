#define CAF_SUITE message_generator

#include "caf/test/dsl.hpp"

#include "actors/message_generator.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture {
  fixture() {
    // nop
  }

  actor_system_config cfg;
  actor_system sys{cfg};
  scoped_actor self{sys};
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(test, fixture)

CAF_TEST(generate 100 messages) {
  auto mg = sys.spawn(actors::message_generator, 0, 0, 100);
  CAF_MESSAGE("Adding self twice as node");
  self->send(mg, add_node_atom_v, self);
  self->send(mg, add_node_atom_v, self);
  CAF_MESSAGE("Starting the message generation");
  self->send(mg, generate_message_atom_v);
  int received = 0;
  self->receive_for(received, 100)([&](message_atom, routing::message) {});
  CAF_CHECK_EQUAL(received, 100);
}

CAF_TEST_FIXTURE_SCOPE_END()
