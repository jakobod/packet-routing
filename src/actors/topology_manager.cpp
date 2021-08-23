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
#include "graph/undirected_graph.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::literals::chrono_literals;

namespace actors {

behavior topology_manager(stateful_actor<topology_manager_state>* self,
                          actor message_generator, actor listener,
                          routing::hyperparameters params, seed_type seed,
                          bool random, std::chrono::milliseconds change_rate) {
  aout(self) << "[topo] has id = " << self->id() << std::endl;
  self->set_down_handler([=](const down_msg& msg) {
    msg.source == listener ? self->quit()
                           : self->state.remove(self, msg.source);
  });
  self->state.gen.seed(seed);
  self->monitor(listener);
  if (change_rate > 0ms)
    self->delayed_send(self, change_rate, change_topology_atom_v);
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions) {
      self->state.num_transitions = num_transitions;
      auto res = graph::generate_random_undirected_graph(num_nodes,
                                                         num_transitions, seed);
      if (res == std::nullopt)
        std::abort();
      auto graph = *res;
      aout(self) << "[topo] Generated graph" << std::endl;
      aout(self) << "[topo] Spawning nodes" << std::endl;
      self->state.next_node_id = graph.nodes().back() + 1;
      for (const auto& node_id : graph.nodes()) {
        auto node = self->spawn(actors::node, node_id, seed, listener, self,
                                params, random);
        self->monitor(node);
        self->delayed_send(node, 100ms, get_load_atom_v);
        self->send(message_generator, add_node_atom_v, node);
        self->state.nodes.emplace_back(std::move(node), node_id);
      }
      aout(self) << "[topo] Spawning transitions" << std::endl;
      for (const auto& trans : graph.transitions()) {
        self->state.transitions.emplace_back(self->spawn(
          actors::transition, self->state.nodes.at(trans.node_1),
          self->state.nodes.at(trans.node_2), self, trans.weight, listener));
        self->monitor(self->state.transitions.back());
      }
      aout(self) << "[topo] Finished building graph with "
                 << graph.num_transitions() << " transitions and "
                 << graph.num_nodes() << " nodes." << std::endl;
    },
    [=](change_topology_atom) {
      std::bernoulli_distribution d(0.5);
      if (d(self->state.gen)) {
        std::cerr << "[topo] ADDING node" << std::endl;
        auto& gen = self->state.gen;
        auto node_id = self->state.next_node_id++;
        // Spawn and setup the new node
        aout(self) << "[topo] Adding node " << node_id << std::endl;
        auto node = self->spawn(actors::node, node_id, seed, listener, self,
                                params, random);
        self->monitor(node);
        self->delayed_send(node, 100ms, get_load_atom_v);
        self->state.nodes.emplace_back(node, node_id);
        // Randomly pick some other nodes and spawn some transitions
        std::uniform_int_distribution<size_t> num_transitions_dist(
          1, (self->state.num_transitions - self->state.transitions.size()));
        std::uniform_int_distribution<size_t> weight_dist(1, 100);
        // Exclude the new node at the end
        std::uniform_int_distribution<size_t> node_index_dist(
          0, self->state.nodes.size() - 2);
        auto num_transitions = num_transitions_dist(gen);
        graph::transition_set transitions;
        aout(self) << "[topo] Adding " << num_transitions << " new transitions"
                   << std::endl;
        while (transitions.size() < num_transitions) {
          graph::transition new_transition{(self->state.nodes.size() - 1),
                                           node_index_dist(gen),
                                           weight_dist(gen)};
          transitions.emplace(std::move(new_transition));
        }
        for (auto& trans : transitions) {
          std::cerr << "[topo] spawning transition with indexes = "
                    << trans.node_1 << " and " << trans.node_2
                    << " with size() = " << self->state.nodes.size()
                    << std::endl;
          self->spawn(actors::transition, self->state.nodes.at(trans.node_1),
                      self->state.nodes.at(trans.node_2), self, trans.weight,
                      listener);
        }
        self->send(message_generator, add_node_atom_v, std::move(node));
      } else {
        std::cerr << "[topo] REMOVING node" << std::endl;
        std::uniform_int_distribution<size_t> dist(0, self->state.nodes.size()
                                                        - 1);
        auto index = dist(self->state.gen);
        std::cerr << "[topo] Going to remove node from index = " << index
                  << std::endl;
        auto& p = self->state.nodes.at(index);
        std::cerr << "[topo] Removing node " << p.second << std::endl;
        self->send_exit(p.first, exit_reason::kill);
      }
      self->delayed_send(self, change_rate, change_topology_atom_v);
    },
    [=](done_atom) {
      auto& state = self->state;
      if (++state.initialized_transitions == state.num_transitions) {
        aout(self) << "[topo] All transitions initialized" << std::endl;
        self->send(message_generator, generate_message_atom_v);
      }
    },
  };
}

} // namespace actors
