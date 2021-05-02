#include "actors/node.hpp"

#include <random>
#include <string>

#include "caf/actor_ostream.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"
<<<<<<< HEAD
#include <random>
#include <vector>
#include <string>
=======
>>>>>>> 5132555dbc03f02b605b46623efd98258945558d

using namespace caf;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self, int node_index, int seed,
                    actor parent) {
  self->state.generator = std::mt19937(seed);
  self->state.node_index = node_index;
  self->link_to(parent);
  self->set_down_handler([=](const down_msg& msg) {
    aout(self) << "[node " << node_index << "]: transition " << msg.source
               << " down. Reason " << msg.reason << std::endl;
    auto& transitions = self->state.transitions;
    transitions.erase(std::remove_if(begin(transitions), end(transitions),
                                     [=](const auto& x) {
                                       return x == msg.source;
                                     }),
                      end(transitions));
  });
<<<<<<< HEAD
  return {[=](register_transition_atom, actor trans) {
            self->state.print(self, "Got new transition " + to_string(trans));
            self->state.transitions.push_back(trans);
            self->monitor(trans);
            return done_atom_v;
          },
          [=](message_atom, routing::message& msg) {
            if (msg.destination() == self) {
              self->state.print(self, "Got message: " + msg.content());
              aout(self) << "[node] Hops: " << msg.path() << std::endl;
            } else {
              //self->state.print(self, "Forwarding message: " + msg.content()
              //                          + " Path lenght: "
              //                          + std::to_string(msg.path_length()));
              msg.update_path(self->state.node_index);
              msg.update_weight(self->state.current_load);

              std::uniform_int_distribution<> distrib(
                0, self->state.transitions.size() - 1);
              auto& trans
                = self->state.transitions.at(distrib(self->state.generator));
              self->send(trans, message_atom_v, std::move(msg));
            }
          }};
=======
  return {
    [=](register_transition_atom, actor trans) {
      aout(self) << "[node " << node_index << "]: Got new transition " << trans
                 << std::endl;
      self->state.transitions.push_back(trans);
      self->monitor(trans);
      return done_atom_v;
    },
    [=](message_atom, routing::message& msg) {
      if (msg.destination() == self) {
        aout(self) << "[node " << node_index
                   << "]: Got message: " << msg.content() << msg.path()
                   << std::endl;
      } else {
        aout(self) << "[node " << node_index
                   << "]: Forwarding message: " + msg.content()
                   << ", Path lenght: " << msg.path_length();
        msg.update_path(self);
        msg.update_weight(self->state.current_load);
        std::uniform_int_distribution<> distrib(
          0, self->state.transitions.size() - 1);
        auto& trans
          = self->state.transitions.at(distrib(self->state.generator));
        self->send(trans, message_atom_v, std::move(msg));
      }
    },
  };
>>>>>>> 5132555dbc03f02b605b46623efd98258945558d
}

} // namespace actors
