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

static constexpr uint8_t add_node = 0;
static constexpr uint8_t remove_node = 1;
static constexpr uint8_t add_transition = 2;
static constexpr uint8_t remove_transition = 3;

struct topology_manager_state {
  static const char* name;
  size_t num_transitions = 0;
  size_t next_node_id = 0;
  std::vector<node_pair> nodes;
  std::vector<caf::actor> transitions;
  size_t initialized_transitions = 0;
  std::mt19937 gen;

  void remove(size_t index) {
    nodes.erase(nodes.begin() + index);
  }

  void remove(caf::stateful_actor<topology_manager_state>* self,
              const caf::actor_addr& addr) {
    // Actor should only be present in one of both lists. We don't know if an
    // actor is a node or a transition on shutdown, hence we try to remove the
    // actor from both lists.
    if (auto it
        = std::remove_if(nodes.begin(), nodes.end(),
                         [=](const auto& p) { return p.first == addr; });
        it != nodes.end()) {
      nodes.erase(it, nodes.end());
      caf::aout(self) << "[topo] Removed node" << std::endl;
    } else {
      transitions.erase(std::remove(transitions.begin(), transitions.end(),
                                    addr),
                        transitions.end());
      caf::aout(self) << "[topo] Removed transition" << std::endl;
    }
  }
};

caf::behavior
topology_manager(caf::stateful_actor<topology_manager_state>* self,
                 caf::actor message_generator, caf::actor listener,
                 routing::hyperparameters params, seed_type seed, bool random,
                 std::chrono::milliseconds change_rate);

} // namespace actors
