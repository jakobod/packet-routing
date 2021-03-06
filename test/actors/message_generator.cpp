#define CAF_SUITE actors.message_generator

#include "caf/test/dsl.hpp"

#include "actors/message_generator.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace {

struct fixture : test_coordinator_fixture<> {
  fixture() {
    // nop
  }
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(message_generator_tests, fixture)

CAF_TEST(generate 100 messages) {
  using namespace std::literals::chrono_literals;
  auto gen = sys.spawn(actors::message_generator, self, 0, 100, 30s);
  CAF_MESSAGE("Adding self as node twice");
  self->send(gen, add_node_atom_v, self);
  self->send(gen, add_node_atom_v, self);
  CAF_MESSAGE("Starting the message generation");
  self->send(gen, generate_message_atom_v);
  run();
  int received = 0;
  self->receive_for(received, 100)([&](message_atom, routing::message) {});
  CAF_CHECK_EQUAL(received, 100);
}

CAF_TEST_FIXTURE_SCOPE_END()
