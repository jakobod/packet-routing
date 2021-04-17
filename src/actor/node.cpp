#include "actor/node.hpp"
#include "caf/all.hpp"
#include "type_ids.hpp"

namespace actor {

caf::behavior node_actor(caf::stateful_actor<node_state>* self) {
  return {[=](register_transition_atom, caf::actor outgoing_transition) {
            self->state.outgoing_transitions.push_back(outgoing_transition);
          },
          [=](message_atom, const std::string& payload) {
            // Nop
            aout(self) << "Got message: " << payload << std::endl;
          },
          [=](emit_message_atom, const std::string& payload) {
            // TODO: Make real stuff happening
            for (const auto& transition : self->state.outgoing_transitions) {
              self->send(transition, message_atom_v, payload);
            }
          }};
}

} // namespace actor