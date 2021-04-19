#include "actors/transition.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/policy/select_all.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace actors {

behavior transition_actor(stateful_actor<transition_state>* self,
                          actor node_one, actor node_two, actor parent) {
  aout(self) << "[transition]: Notifying the other actors" << std::endl;
  std::vector<actor> nodes{node_one, node_two};
  self
    ->fan_out_request<policy::select_all>(nodes, infinite,
                                          register_transition_atom_v, self)
    .then([=](std::vector<done_atom>) { self->send(parent, done_atom_v); });
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
