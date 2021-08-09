#include "graph/generator.hpp"

#include <fstream>
#include <iostream>
#include <random>

#include "boost/graph/graphviz.hpp"

using namespace boost;

namespace graph {

undirected_graph generate_random_graph(size_t num_nodes, size_t num_transitions,
                                       seed_type seed) {
  const auto max_num_transitions = ((num_nodes * (num_nodes - 1)) / 2);
  if (num_transitions > max_num_transitions)
    std::cout << "The number of transitions is too large! with " << num_nodes
              << " nodes there can only be " << max_num_transitions
              << std::endl;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<id_type> dist(0, num_nodes - 1);
  std::uniform_int_distribution<weight_type> edge_dist(0, 100);
  transition_list transitions;
  while (transitions.size() < num_transitions) {
    transition new_transition{dist(gen), dist(gen), edge_dist(gen)};
    // Does the edge connect the same node?
    if ((new_transition.node_1 == new_transition.node_2)
        || std::find(transitions.begin(), transitions.end(), new_transition)
             != transitions.end())
      continue;
    transitions.emplace_back(std::move(new_transition));
  }
  undirected_graph g(num_nodes);
  for (const auto& e : transitions)
    add_edge(e.node_1, e.node_2, e.weight, g);
  return g;
}

node_list get_nodes(const undirected_graph& g) {
  auto index = boost::get(vertex_index, g);
  node_list result;
  for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first)
    result.push_back(index[*vp.first]);
  return result;
}

transition_list get_transitions(const undirected_graph& g) {
  auto vertex_id = boost::get(vertex_index, g);
  auto weightmap = boost::get(boost::edge_weight, g);
  transition_list result;
  for (auto ei = boost::edges(g).first; ei != boost::edges(g).second; ++ei) {
    auto src = boost::source(*ei, g), targ = boost::target(*ei, g);
    auto weight = get(weightmap, *ei);
    auto source = boost::get(vertex_id, src);
    auto target = boost::get(vertex_id, targ);
    result.emplace_back(source, target, weight);
  }
  return result;
}

size_t num_nodes(const undirected_graph& g) {
  return boost::num_vertices(g);
}

size_t num_transitions(const undirected_graph& g) {
  return boost::num_edges(g);
}

void log_graph(const undirected_graph& g, const std::string& path) {
  std::ofstream graph_log;
  graph_log.open(path);
  write_graphviz(graph_log, g);
  graph_log.close();
}

} // namespace graph
