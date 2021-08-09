#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/operators.hpp>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph/edge.hpp"
#include "types.hpp"

namespace graph {

// -- type declarations --------------------------------------------------------

using vertex = id_type;
using undirected_graph
  = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                          boost::no_property,
                          boost::property<boost::edge_weight_t, id_type>>;
using vertex_list = std::vector<vertex>;
using edge_list = std::vector<edge>;

// -- Public API ---------------------------------------------------------------

undirected_graph generate_random_graph(size_t num_verticies, size_t num_edges,
                                       seed_type seed);

vertex_list get_verteces(const undirected_graph& g);

edge_list get_edges(const undirected_graph& g);

size_t num_verteces(const undirected_graph& g);

size_t num_edges(const undirected_graph& g);

void log_graph(const undirected_graph& g,
               const std::string& path = "graph.log");

} // namespace graph
