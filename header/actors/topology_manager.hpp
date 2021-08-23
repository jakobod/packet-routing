/**
 * @author Henri Burau
 * @email henri.burau@haw-hamburg.de
 * @file topology_manager.hpp
 */

#pragma once

#include <random>
#include <vector>

#include "caf/actor.hpp"
#include "caf/actor_addr.hpp"
#include "caf/actor_ostream.hpp"
#include "caf/behavior.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/stateful_actor.hpp"
#include "graph/undirected_graph.hpp"
#include "routing/hyperparameters.hpp"

#include "types.hpp"

namespace actors {

struct topology_manager_state {
  size_t num_transitions = 0;
  size_t next_node_id = 0;
  std::vector<node_pair> nodes;
  std::vector<caf::actor> transitions;
  size_t initialized_transitions = 0;
  std::mt19937 gen;

  bool is_initialized() {
    return ++initialized_transitions == num_transitions;
  }

  void remove(size_t index) {
    nodes.erase(nodes.begin() + index);
  }

  void remove(const caf::actor_addr& addr) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                               [=](const auto& p) { return p.first == addr; }),
                nodes.end());
    transitions.erase(std::remove(transitions.begin(), transitions.end(), addr),
                      transitions.end());
  }
};

caf::behavior
topology_manager(caf::stateful_actor<topology_manager_state>* self,
                 caf::actor message_generator, caf::actor listener,
                 routing::hyperparameters params, seed_type seed, bool random,
                 std::chrono::milliseconds change_rate);

} // namespace actors
