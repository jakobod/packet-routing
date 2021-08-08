#include "actors/node.hpp"

#include <limits>
#include <random>
#include <string>

#include "caf/actor_ostream.hpp"
#include "caf/fwd.hpp"
#include "routing/ant.hpp"
#include "routing/message.hpp"
#include "routing/random.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono_literals;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self, id_type node_id,
                    seed_type seed, actor listener, actor parent,
                    routing::hyperparameters params, bool random) {
  self->state.generator = std::mt19937(seed);
  self->state.node_id = node_id;
  self->state.load_weight = params.load_weight;
  if (random)
    self->state.routing_table = std::make_shared<routing::random>();
  else
    self->state.routing_table = std::make_shared<routing::ant>();
  self->state.routing_table->init(seed, params);
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(parent);
  self->delayed_send(self, 50ms, get_load_atom_v);
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
    [=](register_transition_atom, actor trans, id_type node_id) {
      self->state.transitions.emplace_back(trans, node_id);
      self->monitor(trans);
      return done_atom_v;
    },
    [=](message_atom, routing::message& msg) {
      self->state.routing_table->update(msg);
      ++self->state.message_count;
      if (msg.destination() == self->state.node_id) {
        self->send(listener, message_delivered_atom_v, std::move(msg), true);
      } else {
        msg.update_path(self->state.node_id);
        auto index = self->state.routing_table->get_route(msg.destination());
        if (index == std::numeric_limits<id_type>::max()
            || msg.path_contains(index)) {
          std::shuffle(self->state.transitions.begin(),
                       self->state.transitions.end(), self->state.generator);
          bool sent_message = false;
          for (auto& p : self->state.transitions) {
            if (!msg.path_contains(p.second)
                && p.second != self->state.node_id) {
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
          auto trans = self->state.from_id(index);
          self->delayed_send(
            trans, std::chrono::milliseconds(self->state.current_load),
            message_atom_v, std::move(msg));
        }
      }
    },
    [=](get_load_atom) {
      self->state.current_load = static_cast<load_type>(
        self->state.load_weight * self->state.message_count);
      self->send(listener, share_load_atom_v, self->state.current_load,
                 self->state.node_id);
      self->state.message_count = 0;
      self->delayed_send(self, 100ms, get_load_atom_v);
    },
  };
}

} // namespace actors
