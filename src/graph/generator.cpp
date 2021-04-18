#include "graph/generator.hpp"
#include <random>

using namespace boost;

namespace graph {

Graph generate_random_graph(size_t num_verticies, size_t num_edges, int seed) {
  size_t i = 0, j = 0, count = 0;
  std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, num_verticies);

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

} // namespace graph
