#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/operators.hpp>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph/transition.hpp"
#include "types.hpp"

namespace graph {

// -- type declarations --------------------------------------------------------

using node = id_type;
using undirected_graph
  = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                          boost::no_property,
                          boost::property<boost::edge_weight_t, id_type>>;
using node_list = std::vector<node>;
using transition_list = std::vector<transition>;

// -- Public API ---------------------------------------------------------------

undirected_graph generate_random_graph(size_t num_nodes, size_t num_transitions,
                                       seed_type seed);

node_list get_nodes(const undirected_graph& g);

transition_list get_transitions(const undirected_graph& g);

size_t num_nodes(const undirected_graph& g);

size_t num_transitions(const undirected_graph& g);

void log_graph(const undirected_graph& g,
               const std::string& path = "graph.log");

} // namespace graph
