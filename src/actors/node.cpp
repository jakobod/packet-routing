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
  self->state.load_weight = params.load_weight;
  if (random)
    self->state.routing_table = std::make_shared<routing::random>();
  else
    self->state.routing_table = std::make_shared<routing::ant>();
  self->state.routing_table->init(seed, params);
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(parent);
  self->delayed_send(self, std::chrono::milliseconds(50), get_load_atom_v);
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
      self->state.messages_visited++;
      if (msg.destination() == self->state.node_index) {
        self->send(listener, message_delivered_atom_v, std::move(msg), true);
      } else {
        msg.update_path(self->state.node_index);
        auto index = self->state.routing_table->get_route(msg.destination());
        if (index < 0 || msg.path_contains(index)) {
          std::shuffle(self->state.transitions.begin(),
                       self->state.transitions.end(), self->state.generator);
          bool sent_message = false;
          for (auto& p : self->state.transitions) {
            if (!msg.path_contains(p.second)
                && p.second != self->state.node_index) {
              sent_message = true;
              self->delayed_send(
                p.first, std::chrono::milliseconds(self->state.current_load),
                message_atom_v, std::move(msg));
              break;
            }
          }
          if (!sent_message)
            self->send(listener, message_delivered_atom_v, std::move(msg),
                       false);
        } else {
          auto trans = self->state.from_index(index);
          self->delayed_send(
            trans, std::chrono::milliseconds(self->state.current_load),
            message_atom_v, std::move(msg));
        }
      }
    },
    [=](get_load_atom) {
      self->state.current_load
        = uint64_t(self->state.messages_visited * self->state.load_weight);
      self->send(listener, share_load_atom_v, self->state.current_load,
                 self->state.node_index);
      self->state.messages_visited = 0;
      self->delayed_send(self, std::chrono::milliseconds(100), get_load_atom_v);
    },
  };
}

} // namespace actors
