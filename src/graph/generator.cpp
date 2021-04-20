#include "graph/generator.hpp"
#include <random>

using namespace boost;

namespace graph {

Graph generate_random_graph(size_t num_verticies, size_t num_edges, int seed) {
  size_t i = 0, j = 0, count = 0;
  std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, num_verticies - 1);

  Edge edges[num_edges];

  while (i < num_edges) {
    edges[i] = Edge(distrib(gen), distrib(gen));

    if (edges[i].first == edges[i].second)
      continue;
    else {
      for (j = 0; j < i; j++) {
        if ((edges[i].first == edges[j].first
             && edges[i].second == edges[j].second)
            || (edges[i].first == edges[j].second
                && edges[i].second == edges[j].first))
          i--;
      }
    }
    i++;
  }

  Graph g(num_verticies);

  for (i = 0; i < num_edges; ++i) {
    add_edge(edges[i].first, edges[i].second, g);
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

} // namespace graph
