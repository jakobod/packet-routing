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
                          actor message_generator, actor benchmarker,
                          routing::hyperparameters params, seed_type seed,
                          bool random, std::chrono::milliseconds change_rate) {
  aout(self) << "[topo] has id = " << self->id() << std::endl;
  self->set_down_handler([=](const down_msg&) { self->quit(); });
  self->monitor(benchmarker);
  self->state.gen.seed(seed);
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions) {
      self->state.num_transitions = num_transitions;
      auto res = graph::generate_random_undirected_graph(num_nodes,
                                                         num_transitions, seed);
      if (res == std::nullopt)
        std::abort();
      auto graph = *res;
      aout(self) << "[topo] Generated graph with " << graph.num_nodes()
                 << " nodes and " << graph.num_transitions() << " transitions"
                 << std::endl;
      aout(self) << "[topo] Spawning " << graph.num_nodes() << " nodes"
                 << std::endl;
      self->state.next_node_id = graph.nodes().back() + 1;
      for (const auto& node_id : graph.nodes()) {
        auto node = self->spawn(actors::node, node_id, seed, benchmarker, self,
                                params, random);
        self->delayed_send(node, 100ms, get_load_atom_v);
        self->send(message_generator, add_node_atom_v, node);
        self->state.nodes.emplace_back(std::move(node), node_id);
      }
      aout(self) << "[topo] Spawning " << num_transitions << " transitions"
                 << std::endl;
      for (const auto& trans : graph.transitions()) {
        if (trans.alive)
          self->state.living_transitions.emplace_back(
            self->spawn(actors::transition, self->state.nodes.at(trans.node_1),
                        self->state.nodes.at(trans.node_2), self, trans.weight,
                        benchmarker, trans.alive));
        else
          self->state.dead_transitions.emplace_back(
            self->spawn(actors::transition, self->state.nodes.at(trans.node_1),
                        self->state.nodes.at(trans.node_2), self, trans.weight,
                        benchmarker, trans.alive));
      }
      aout(self) << "[topo] Finished building topology" << std::endl;
    },
    [=](change_topology_atom) {
      auto& living_transitions = self->state.living_transitions;
      auto& dead_transitions = self->state.dead_transitions;
      std::bernoulli_distribution d(0.5);
      auto kill = d(self->state.gen);
      if (kill && !living_transitions.empty()) {
        std::uniform_int_distribution<size_t> dist(0, living_transitions.size()
                                                        - 1);
        auto index = dist(self->state.gen);
        auto& trans = living_transitions.at(index);
        self->send(trans, kill_atom_v);
        dead_transitions.emplace_back(std::move(trans));
        living_transitions.erase(living_transitions.begin() + index);
      } else if (!dead_transitions.empty()) {
        std::uniform_int_distribution<size_t> dist(0,
                                                   dead_transitions.size() - 1);
        auto index = dist(self->state.gen);
        auto& trans = dead_transitions.at(index);
        self->send(trans, resurrect_atom_v);
        living_transitions.emplace_back(std::move(trans));
        dead_transitions.erase(dead_transitions.begin() + index);
      }
      self->delayed_send(self, change_rate, change_topology_atom_v);
    },
    [=](done_atom) {
      if (self->state.is_initialized()) {
        self->send(message_generator, generate_message_atom_v);
        if (change_rate > 0ms)
          self->delayed_send(self, change_rate, change_topology_atom_v);
      }
    },
  };
}

} // namespace actors
