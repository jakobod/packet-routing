#include "graph/generator.hpp"
#include <random>

using namespace boost;

namespace graph {

Graph generate_random_graph(size_t num_verticies, size_t num_edges, int seed) {
  size_t i = 0, j = 0, count = 0;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distrib(0, num_verticies - 1);
  std::uniform_int_distribution<> edge_distrib(0, 100);

  Edge edges[num_edges];

  while (i < num_edges) {
    edges[i] = std::make_tuple(distrib(gen), distrib(gen), edge_distrib(gen));

    if (std::get<0>(edges[i]) == std::get<1>(edges[i]))
      continue;
    else {
      for (j = 0; j < i; j++) {
        if ((std::get<0>(edges[i]) == std::get<0>(edges[j])
             && std::get<1>(edges[i]) == std::get<1>(edges[j]))
            || (std::get<0>(edges[i]) == std::get<1>(edges[j])
                && std::get<1>(edges[i]) == std::get<0>(edges[j])))
          i--;
      }
    }
    i++;
  }

  Graph g(num_verticies);

  for (i = 0; i < num_edges; ++i) {
    add_edge(std::get<0>(edges[i]), std::get<1>(edges[i]),
             std::get<2>(edges[i]), g);
  }

  return g;
}

std::vector<Vertex> get_verteces(Graph graph) {
  std::vector<Vertex> result;
  typedef property_map<Graph, vertex_index_t>::type IndexMap;
  IndexMap index = boost::get(vertex_index, graph);

  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;

  for (vp = boost::vertices(graph); vp.first != vp.second; ++vp.first)
    result.push_back(index[*vp.first]);

  return result;
}

std::vector<Edge> get_edges(Graph graph) {
  std::vector<Edge> result;
  boost::property_map<Graph, boost::edge_weight_t>::type weightmap;
  typename property_map<Graph, vertex_index_t>::type vertex_id
    = boost::get(vertex_index, graph);

  weightmap = boost::get(boost::edge_weight, graph);

  boost::graph_traits<Graph>::edge_iterator ei;
  for (ei = boost::edges(graph).first; ei != boost::edges(graph).second; ++ei) {
    Vertex src = boost::source(*ei, graph), targ = boost::target(*ei, graph);
    int weight = get(weightmap, *ei);
    int source = boost::get(vertex_id, src);
    int target = boost::get(vertex_id, targ);

    Edge edge = std::make_tuple(source, target, weight);
    result.push_back(edge);
  }

  return result;
}

int num_verteces(Graph graph) {
  return boost::num_vertices(graph);
}

int num_edges(Graph graph) {
  return boost::num_edges(graph);
}

} // namespace graph
