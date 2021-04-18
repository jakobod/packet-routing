#include "actors/transition.hpp"

#include "caf/all.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self,
                               caf::actor node_one, caf::actor node_two) {
  self->send(node_one, register_transition_atom_v, self);
  self->send(node_two, register_transition_atom_v, self);

  return {
    [=](message_atom, const std::string& payload) {
      if (self->current_sender() == node_one) {
        self->send(node_two, std::move(payload));
      } else {
        self->send(node_one, std::move(payload));
      }
    },
  };
}

} // namespace actors
