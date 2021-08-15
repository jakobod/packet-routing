/**
 * @author Henri Burau
 * @email henri.burau@haw-hamburg.de
 * @file topology_manager.cpp
 */

#include "actors/topology_manager.hpp"

#include <chrono>

#include "actors/node.hpp"
#include "actors/transition.hpp"
#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/stateful_actor.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::literals::chrono_literals;

namespace actors {

behavior topology_manager(stateful_actor<topology_manager_state>* self,
                          actor message_generator, actor listener,
                          routing::hyperparameters params, bool random) {
  self->set_down_handler([=](const down_msg&) { self->quit(); });
  self->monitor(listener);

  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions,
        seed_type seed) {
      auto res = graph::generate_random_undirected_graph(num_nodes,
                                                         num_transitions, seed);
      if (res == std::nullopt)
        std::abort();
      auto graph = *res;
      aout(self) << "[topo] Generated graph" << std::endl;
      aout(self) << "[topo] Spawning nodes" << std::endl;
      for (const auto& node_id : graph.nodes()) {
        auto node = self->spawn(actors::node, node_id, seed, listener, self,
                                params, random);
        self->delayed_send(node, 100ms, get_load_atom_v);
        self->send(message_generator, add_node_atom_v, node);
        self->state.nodes.emplace(node_id, std::move(node));
      }
      aout(self) << "[topo] Spawning transitions" << std::endl;
      for (const auto& trans : graph.transitions()) {
        auto node_one = std::make_pair(self->state.nodes.at(trans.node_1),
                                       trans.node_1);
        auto node_two = std::make_pair(self->state.nodes.at(trans.node_2),
                                       trans.node_2);
        self->state.transitions.emplace(
          std::make_pair(trans.node_1, trans.node_2),
          self->spawn(actors::transition, node_one, node_two, self,
                      trans.weight, listener));
      }
      aout(self) << "[topo] Finished building graph with "
                 << graph.num_transitions() << " transitions and "
                 << graph.num_nodes() << " nodes." << std::endl;
    },
    [=](done_atom) {
      auto& state = self->state;
      if (++state.initialized_transitions >= state.transitions.size()) {
        aout(self) << "[topo] Transitions initialized" << std::endl;
        self->send(message_generator, generate_message_atom_v);
      }
    },
  };
}

} // namespace actors
