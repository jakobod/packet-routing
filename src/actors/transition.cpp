#include "actors/transition.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace actors {

behavior transition_actor(stateful_actor<transition_state>* self,
                          actor node_one, actor node_two, actor parent) {
  aout(self) << "[transition]: Notifying the other actors" << std::endl;
  self->request(node_one, seconds(1), register_transition_atom_v, self)
    .then([=](done_atom d) { self->send(parent, d); });
  self->request(node_two, seconds(1), register_transition_atom_v, self)
    .then([=](done_atom d) { self->send(parent, d); });
  return {
    [=](message_atom, const std::string& payload) {
      aout(self) << "[transition]: Got new message" << std::endl;
      if (self->current_sender() == node_one) {
        aout(self) << "[transition]: Sending to node2" << std::endl;
        self->send(node_two, message_atom_v, std::move(payload));
      } else {
        aout(self) << "[transition]: Sending to node1" << std::endl;
        self->send(node_one, std::move(payload));
      }
    },
  };
}

} // namespace actors
