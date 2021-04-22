#include "actors/transition.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/policy/select_all.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace actors {

behavior transition_actor(stateful_actor<transition_state>* self,
                          actor node_one, actor node_two, actor parent,
                          uint64_t weight) {
  std::vector<actor> nodes{node_one, node_two};
  self->link_to(parent);
  self
    ->fan_out_request<policy::select_all>(nodes, infinite,
                                          register_transition_atom_v, self)
    .then([=](std::vector<done_atom>) { self->send(parent, done_atom_v); });
  self->state.weight = weight;
  return {
    [=](message_atom, routing::message& msg) {
      aout(self) << "[transition]: forwarding message" << std::endl;
      msg.update_weight(self->state.weight);
      if (self->current_sender() == node_one)
        self->send(node_two, message_atom_v, std::move(msg));
      else
        self->send(node_one, std::move(msg));
    },
  };
}

} // namespace actors

