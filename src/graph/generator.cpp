#include "graph/generator.hpp"

#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <random>

using namespace boost;

namespace graph {

undirected_graph generate_random_graph(size_t num_verticies, size_t num_edges,
                                       int seed) {
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distrib(0, num_verticies - 1);
  std::uniform_int_distribution<> edge_distrib(0, 100);
  edge_list edges;

  while (edges.size() < num_edges) {
    edge new_edge{distrib(gen), distrib(gen), edge_distrib(gen)};
    // Does the edge connect the same node?
    if ((new_edge.node_1 == new_edge.node_2)
        || std::find(std::begin(edges), std::end(edges), new_edge)
             != edges.end())
      continue;
    edges.emplace_back(std::move(new_edge));
  }
  undirected_graph g(num_verticies);
  for (const auto& e : edges)
    add_edge(e.node_1, e.node_2, e.weight, g);
  return g;
}

vertex_list get_verteces(const undirected_graph& g) {
  using index_map = property_map<undirected_graph, vertex_index_t>::type;
  using vertex_iter = graph_traits<undirected_graph>::vertex_iterator;
  using vertex_iter_pair = std::pair<vertex_iter, vertex_iter>;
  auto index = boost::get(vertex_index, g);
  vertex_list result;
  for (auto vp = boost::vertices(g); vp.first != vp.second; ++vp.first)
    result.push_back(index[*vp.first]);
  return result;
}

edge_list get_edges(const undirected_graph& g) {
  std::vector<edge> result;
  auto vertex_id = boost::get(vertex_index, g);
  auto weightmap = boost::get(boost::edge_weight, g);

  for (auto ei = boost::edges(g).first; ei != boost::edges(g).second; ++ei) {
    auto src = boost::source(*ei, g), targ = boost::target(*ei, g);
    auto weight = get(weightmap, *ei);
    auto source = boost::get(vertex_id, src);
    auto target = boost::get(vertex_id, targ);
    result.emplace_back(source, target, weight);
  }
  return result;
}

size_t num_verteces(const undirected_graph& g) {
  return boost::num_vertices(g);
}

size_t num_edges(const undirected_graph& g) {
  return boost::num_edges(g);
}

void log_graph(const undirected_graph& g, const std::string& path) {
  std::ofstream graph_log;
  graph_log.open(path);
  write_graphviz(graph_log, g);
  graph_log.close();
}

} // namespace graph
