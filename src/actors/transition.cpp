#include "actors/transition.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/policy/select_all.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior transition_actor(caf::stateful_actor<transition_state>* self,
                          node_pair node_1, node_pair node_2, caf::actor parent,
                          weight_type weight, caf::actor) {
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(parent);
  self->state.index = std::make_pair(node_1.second, node_2.second);
  self->send(node_1.first, register_transition_atom_v, self, node_2.second);
  self->send(node_2.first, register_transition_atom_v, self, node_1.second);
  self->state.weight = weight;

  return {
    [=](message_atom, routing::message& msg) {
      msg.update_weight(self->state.weight);
      if (self->current_sender() == node_1.first)
        self->delayed_send(node_2.first,
                           std::chrono::milliseconds(self->state.weight),
                           message_atom_v, std::move(msg));
      else
        self->delayed_send(node_1.first,
                           std::chrono::milliseconds(self->state.weight),
                           message_atom_v, std::move(msg));
    },
    [=](done_atom) {
      if (++self->state.received_dones == 2)
        self->send(parent, done_atom_v);
    },
  };
}

} // namespace actors
