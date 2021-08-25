#include "actors/transition.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/policy/select_all.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

const char* transition_state::name = "transition";

behavior killed_transition(caf::stateful_actor<transition_state>* self,
                           caf::actor listener) {
  self->set_default_handler(drop);
  aout(self) << "[transition] killed" << std::endl;
  return {
    [=](message_atom, routing::message& msg) {
      aout(self) << "[transition] dropped message" << std::endl;
      self->send(listener, message_dropped_atom_v, msg);
    },
    [=](resurrect_atom) {
      aout(self) << "[transition] resurrecting" << std::endl;
      self->unbecome();
    },
  };
}

behavior transition(caf::stateful_actor<transition_state>* self,
                    node_pair node_1, node_pair node_2, caf::actor parent,
                    weight_type weight, caf::actor listener, bool alive) {
  self->set_down_handler([=](const down_msg&) { self->quit(); });
  self->monitor(parent);
  self->monitor(node_1.first);
  self->monitor(node_2.first);
  self->send(node_1.first, register_transition_atom_v, self, weight,
             node_2.second);
  self->send(node_2.first, register_transition_atom_v, self, weight,
             node_1.second);
  self->state.weight = weight;
  if (!alive)
    self->become(killed_transition(self, listener));
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
    [=](kill_atom) { self->become(killed_transition(self, listener)); },
    [=](resurrect_atom) {
      // nop
    },
    [=](done_atom) {
      if (++self->state.received_dones == 2)
        self->send(parent, done_atom_v);
    },
  };
}

} // namespace actors
