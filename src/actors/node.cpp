#include "actors/node.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self, actor parent) {
  self->link_to(parent);
  self->set_down_handler([=](const down_msg& msg) {
    aout(self) << "down: transition " << msg.source << " down. Reason "
               << msg.reason << std::endl;
    auto& transitions = self->state.transitions;
    transitions.erase(std::remove_if(begin(transitions), end(transitions),
                                     [=](const auto& x) {
                                       return x == msg.source;
                                     }),
                      end(transitions));
  });
  return {[=](register_transition_atom, actor trans) {
            aout(self) << "[node]: Got new transition " << trans << std::endl;
            self->state.transitions.push_back(trans);
            self->monitor(trans);
            return done_atom_v;
          },
          [=](message_atom, routing::message& msg) {
            if (msg.destination() == self) {
              aout(self) << "[node]: got message: " << msg << std::endl;
            } else {
              aout(self) << "[node]: forwarding message" << std::endl;
              msg.update_path(self);
              msg.update_weight(self->state.current_load);
              // TODO: Update current load accordingly.
              // TODO: this should be routed according to routing table
              for (const auto& trans : self->state.transitions) {
                if (trans != self->current_sender())
                  self->send(trans, message_atom_v, std::move(msg));
              }
            }
          },
          [=](emit_message_atom, routing::message& msg) {
            aout(self) << "[node]: emitting message" << std::endl;
            // TODO: Make real stuff happening
            msg.update_path(self);
            msg.update_weight(self->state.current_load);
            for (const auto& transition : self->state.transitions)
              self->send(transition, message_atom_v, std::move(msg));
          }};
}

} // namespace actors
