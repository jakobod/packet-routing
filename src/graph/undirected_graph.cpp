/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file undirected_graph.cpp
 */

#include "graph/undirected_graph.hpp"

#include <iostream>
#include <random>

namespace graph {

undirected_graph::undirected_graph(node_list nodes, transition_list transitions)
  : nodes_{std::move(nodes)}, transitions_{std::move(transitions)} {
  // nop
}

const node_list& undirected_graph::nodes() const {
  return nodes_;
}

const transition_list& undirected_graph::transitions() const {
  return transitions_;
}

size_t undirected_graph::num_nodes() const {
  return nodes_.size();
}

size_t undirected_graph::num_transitions() const {
  return transitions_.size();
}

std::optional<undirected_graph>
generate_random_undirected_graph(size_t num_nodes, size_t num_transitions,
                                 seed_type seed) {
  const auto max_num_transitions = ((num_nodes * (num_nodes - 1)) / 2);
  if (num_transitions > max_num_transitions) {
    std::cerr << "The number of transitions is too large! with " << num_nodes
              << " nodes there can only be " << max_num_transitions
              << std::endl;
    return std::nullopt;
  }
  // Generate a list of nodes
  node_list nodes(num_nodes);
  std::iota(nodes.begin(), nodes.end(), 0);
  // Generate random transitions
  std::mt19937 gen(seed);
  std::uniform_int_distribution<id_type> dist(0, num_nodes - 1);
  std::uniform_int_distribution<weight_type> edge_dist(1, 100);
  transition_set transitions;
  // Add all transitions that should be alive
  while (transitions.size() < num_transitions) {
    transition new_transition{dist(gen), dist(gen), edge_dist(gen), true};
    if (new_transition.node_1 == new_transition.node_2)
      continue;
    transitions.emplace(std::move(new_transition));
  }
  // Now fill up with dead transitions
  for (id_type node_1 = 0; node_1 < num_nodes; ++node_1) {
    for (id_type node_2 = 0; node_2 < num_nodes; ++node_2) {
      if (node_1 == node_2)
        continue;
      transitions.emplace(node_1, node_2, edge_dist(gen), false);
    }
  }
  return undirected_graph{std::move(nodes), transition_list{transitions.begin(),
                                                            transitions.end()}};
}

} // namespace graph
