#pragma once

#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/operators.hpp>
#include <iostream>
#include <utility>

namespace graph {

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, boost::no_property, boost::property<boost::edge_weight_t, int>>
  Graph;
typedef std::tuple<int, int, int> Edge;

typedef int Vertex;

Graph generate_random_graph(size_t num_verticies, size_t num_edges, int seed);

std::vector<Vertex> get_verteces(Graph graph);

std::vector<Edge> get_edges(Graph graph);

int num_verteces(Graph graph);

int num_edges(Graph graph);
} // namespace graph
