#include "actors/topology_manager.hpp"

#include <boost/graph/graphviz.hpp>
#include <sstream>

#include "actors/node.hpp"
#include "actors/transition.hpp"
#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/local_actor.hpp"
#include "caf/stateful_actor.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior topology_manager_actor(stateful_actor<topology_manager_state>* self, actor message_generator) {
  self->state.initialized_transitions = 0;

  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions, int seed) {
      auto graph = graph::generate_random_graph(num_nodes, num_transitions,
                                                seed);

      self->state.graph = graph;
      std::ofstream graph_log;
      graph_log.open("graph.log");

      boost::write_graphviz(graph_log, graph);
      graph_log.close();
      aout(self) << "[topo] Generated graph. Written to graph.log."
                 << std::endl;

      aout(self) << "[topo] Adding nodes" << std::endl;
      for (int node : graph::get_verteces(graph)) {
        auto node_ref =  self->spawn(node_actor, node, seed, self);
        self->state.nodes[node] = node_ref;
        self->send(message_generator, add_node_atom_v, node_ref);
      }

      aout(self) << "[topo] Adding transitions" << std::endl;
      for (auto const& edge : graph::get_edges(graph)) {
        auto[first, second, weight] = edge;
        auto node_one = self->state.nodes[first];
        auto node_two = self->state.nodes[second];
        self->state.transitions[std::make_pair(first, second)] = self->spawn(
          transition_actor, node_one, first, node_two, second, self, 10);
      }

      aout(self) << "[topo] Finished building graph with "
                 << graph::num_edges(graph) << " edges and "
                 << graph::num_verteces(graph) << " verteces." << std::endl;
    },
    [=](done_atom) {
      auto& state = self->state;
      state.initialized_transitions++;
      if (state.initialized_transitions >= graph::num_edges(state.graph)) {
        aout(self) << "[topo] Transitions initialized" << std::endl;
      }
    }};
}

} // namespace actors
