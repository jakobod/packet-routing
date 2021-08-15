/**
 * @author Henri Burau
 * @email henri.burau@haw-hamburg.de
 * @file topology_manager.cpp
 */

#include "actors/topology_manager.hpp"

#include "actors/node.hpp"
#include "actors/transition.hpp"
#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/stateful_actor.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior topology_manager(stateful_actor<topology_manager_state>* self,
                          actor message_generator, actor listener,
                          routing::hyperparameters params, bool random,
                          bool log_graph) {
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(listener);

  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions,
        seed_type seed) {
      self->state.graph = graph::generate_random_graph(num_nodes,
                                                       num_transitions, seed);
      auto& g = self->state.graph;
      if (log_graph)
        graph::log_graph(g);
      aout(self) << "[topo] Generated graph. Written to graph.log."
                 << std::endl;
      aout(self) << "[topo] Adding nodes" << std::endl;
      for (const auto& node : graph::get_nodes(g)) {
        auto node_ref = self->spawn(actors::node, node, seed, listener, self,
                                    params, random);
        self->send(message_generator, add_node_atom_v, node_ref);
        self->state.nodes.emplace(node, std::move(node_ref));
      }
      aout(self) << "[topo] Adding transitions" << std::endl;
      for (const auto& trans : graph::get_transitions(g)) {
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
                 << graph::num_transitions(g) << " transitions and "
                 << graph::num_nodes(g) << " nodes." << std::endl;
    },
    [=](done_atom) {
      auto& state = self->state;
      if (++state.initialized_transitions
          >= graph::num_transitions(state.graph)) {
        aout(self) << "[topo] Transitions initialized" << std::endl;
        self->send(message_generator, generate_message_atom_v);
      }
    },
  };
}

} // namespace actors
