#include "actors/node.hpp"

#include "caf/all.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

caf::behavior node_actor(caf::stateful_actor<node_state>* self) {
  return {[=](register_transition_atom, caf::actor trans) {
            aout(self) << "[node]: Got new transition " << trans << std::endl;
            self->state.outgoing_transitions.push_back(trans);
            return done_atom_v;
          },
          [=](message_atom, const std::string& payload) {
            // Nop
            aout(self) << "[node]: got message: " << payload << std::endl;
          },
          [=](emit_message_atom, const std::string& payload) {
            aout(self) << "[node]: emitting message" << std::endl;
            // TODO: Make real stuff happening
            for (const auto& transition : self->state.outgoing_transitions) {
              self->send(transition, message_atom_v, payload);
            }
          }};
}

} // namespace actors
