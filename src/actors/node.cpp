#include "actors/node.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"
#include <random>
#include <string>

using namespace caf;

namespace actors {

void node_state::print(stateful_actor<node_state>* actor_stuff,
                       std::string msg) {
  aout(actor_stuff) << "[node " << node_index << "]: " << msg << std::endl;
}

behavior node_actor(stateful_actor<node_state>* self, int node_index, int seed,
                    actor parent) {
  self->state.generator = std::mt19937(seed);
  self->state.node_index = node_index;
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
            self->state.print(self, "Got new transition " + to_string(trans));
            self->state.transitions.push_back(trans);
            self->monitor(trans);
            return done_atom_v;
          },
          [=](message_atom, routing::message& msg) {
            if (msg.destination() == self) {
              self->state.print(self, "Got message: " + msg.content());
            } else {
              // self->state.print(self, "Forwarding message: " + msg.content()
              //                          + " Path lenght: "
              //                          + std::to_string(msg.path_length()));
              msg.update_path(self);
              msg.update_weight(self->state.current_load);

              std::uniform_int_distribution<> distrib(
                0, self->state.transitions.size() - 1);
              auto& trans
                = self->state.transitions.at(distrib(self->state.generator));
              self->send(trans, message_atom_v, std::move(msg));
            }
          }};
}

} // namespace actors
