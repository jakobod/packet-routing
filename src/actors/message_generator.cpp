#include "actors/message_generator.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"
#include "types.hpp"

using namespace caf;

namespace actors {

const char* message_generator_state::name = "message_generator";

behavior message_generator(stateful_actor<message_generator_state>* self,
                           seed_type seed, size_t num_messages) {
  aout(self) << "[message_generator] has id = " << self->id() << std::endl;
  self->set_down_handler([=](const down_msg& msg) {
    aout(self) << "[message_generator] Removing node" << std::endl;
    self->state.remove_node(msg.source);
  });
  self->state.gen.seed(seed);
  return {
    [=](generate_message_atom) {
      auto& state = self->state;
      if (state.nodes.size() >= 2) {
        std::uniform_int_distribution<id_type> dist(0, state.nodes.size() - 1);
        auto source = dist(state.gen);
        id_type destination = 0;
        while ((destination = dist(state.gen)) == source)
          ;
        routing::message msg(self->state.num_messages, source, destination);
        self->send(state.nodes.at(source), message_atom_v, msg);
      }
      self->send(self, generate_message_atom_v);
      if (++self->state.num_messages >= num_messages)
        self->quit();
    },
    [=](remove_node_atom, const actor& node) { self->state.remove_node(node); },
    [=](add_node_atom, const actor& node) {
      self->monitor(node);
      self->state.nodes.push_back(node);
    },
  };
}

} // namespace actors
