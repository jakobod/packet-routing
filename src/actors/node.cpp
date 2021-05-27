#include "actors/node.hpp"

#include <random>
#include <string>

#include "caf/actor_ostream.hpp"
#include "caf/fwd.hpp"
#include "routing/ant.hpp"
#include "routing/message.hpp"
#include "routing/random.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self, int node_index, int seed,
                    actor listener, actor parent,
                    routing::hyperparameters params, bool random) {
  self->state.generator = std::mt19937(seed);
  self->state.node_index = node_index;
  if (random)
    self->state.routing_table = std::make_shared<routing::random>();
  else
    self->state.routing_table = std::make_shared<routing::ant>();
  self->state.routing_table->init(seed, params);
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(parent);
  self->set_down_handler([=](const down_msg& msg) {
    auto& transitions = self->state.transitions;
    auto node_id = self->state.from_act(msg.source);
    self->state.routing_table->delete_route(node_id);
    transitions.erase(std::remove_if(begin(transitions), end(transitions),
                                     [=](const auto& p) {
                                       return p.first == msg.source;
                                     }),
                      end(transitions));
  });
  return {
    [=](register_transition_atom, actor trans, int node_id) {
      self->state.transitions.emplace_back(trans, node_id);
      self->monitor(trans);
      return done_atom_v;
    },
    [=](message_atom, routing::message& msg) {
      self->state.routing_table->update(msg);
      if (msg.destination() == self->state.node_index) {
        self->send(listener, message_delivered_atom_v, std::move(msg));
      } else {
        msg.update_path(self->state.node_index);
        auto index = self->state.routing_table->get_route(msg.destination());
        if (index < 0 || msg.path_contains(index)) {
          self->send(self->state.pick_random(), message_atom_v, std::move(msg));
        } else {
          auto trans = self->state.from_index(index);
          self->send(trans, message_atom_v, std::move(msg));
        }
      }
    },
  };
}

} // namespace actors
