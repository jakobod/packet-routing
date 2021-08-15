/**
 * @author Henri Burau
 * @email henri.burau@haw-hamburg.de
 * @file topology_manager.hpp
 */

#pragma once

#include <random>
#include <vector>

#include "caf/behavior.hpp"
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
  size_t num_transitions = 0;
  size_t next_node_id = 0;
  std::vector<node_pair> nodes;
  size_t initialized_transitions = 0;
  std::mt19937 gen;

  void add_node() {
    // nop
  }

  void remove_node(size_t index) {
    nodes.erase(nodes.begin() + index);
  }

  void remove_node(const caf::actor_addr& addr) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                               [=](const auto& p) { return p.first == addr; }),
                nodes.end());
  }
};

caf::behavior
topology_manager(caf::stateful_actor<topology_manager_state>* self,
                 caf::actor message_generator, caf::actor listener,
                 routing::hyperparameters params, seed_type seed, bool random,
                 std::chrono::milliseconds change_rate);

} // namespace actors
