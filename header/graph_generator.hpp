#pragma once

#include <algorithm> // for std::for_each
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/operators.hpp>
#include <iostream> // for std::cout
#include <utility>  // for std::pair

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>
  Graph;
typedef std::pair<int, int> Edge;

Graph generate_random_graph(size_t num_verticies, size_t num_edges, int seed);
