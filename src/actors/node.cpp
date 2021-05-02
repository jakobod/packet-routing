#include "actors/node.hpp"

#include <random>
#include <string>

#include "caf/actor_ostream.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self, int node_index, int seed,
                    actor parent) {
  self->state.generator = std::mt19937(seed);
  self->state.node_index = node_index;
  self->state.routing_table.init(seed);
  self->link_to(parent);
  self->set_down_handler([=](const down_msg& msg) {
    aout(self) << "[node " << node_index << "]: transition " << msg.source
               << " down. Reason " << msg.reason << std::endl;
    auto& transitions = self->state.transitions;

    auto node_id = self->state.from_act(msg.source);
    self->state.routing_table.delete_route(node_id);

    transitions.erase(std::remove_if(begin(transitions), end(transitions),
                                     [=](const auto& p) {
                                       return p.first == msg.source;
                                     }),
                      end(transitions));
  });
  return {
    [=](register_transition_atom, actor trans, int node_id) {
      aout(self) << "[node " << node_index << "]: Got new transition " << trans
                 << std::endl;
      self->state.transitions.emplace_back(trans, node_id);
      self->monitor(trans);
      return done_atom_v;
    },
    [=](message_atom, routing::message& msg) {
      self->state.routing_table.update(msg);
      if (msg.destination() == self->state.node_index) {
        aout(self) << "[node " << node_index
                   << "]: Got message: " << msg.content() << msg.path()
                   << std::endl;
      } else {
        aout(self) << "[node " << node_index
                   << "]: Forwarding message: " + msg.content()
                   << ", Last weight: " << msg.last_weight() << std::endl;
        msg.update_path(self->state.node_index);
        auto index = self->state.routing_table.get_route(msg.destination());
        if (index < 0 || msg.path_contains(index)) {
          aout(self) << "[node " << node_index << "]: random" << std::endl;
          self->send(self->state.pick_random(), message_atom_v, std::move(msg));
        } else {
          aout(self) << "[node " << node_index << "]: routing" << std::endl;
          auto trans = self->state.from_index(index);
          self->send(trans, message_atom_v, std::move(msg));
        }
      }
    },
  };
}

} // namespace actors
