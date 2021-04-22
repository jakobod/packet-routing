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

behavior topology_manager_actor(stateful_actor<topology_manager_state>* self) {
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
        self->state.nodes[node] = self->spawn(node_actor, node, self);
      }

      aout(self) << "[topo] Adding transitions" << std::endl;
      for (auto const& edge : graph::get_edges(graph)) {
        EdgeIndex index = std::make_pair(std::get<0>(edge), std::get<1>(edge));
        auto node_one = self->state.nodes[index.first];
        auto node_two = self->state.nodes[index.second];
        self->state.transitions[index] = self->spawn(
          transition_actor, node_one, index.first, node_two, index.second, self, 10);
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
        routing::message msg("Hello World!", self->state.nodes[0]);
        self->send(self->state.nodes[3], emit_message_atom_v, msg);
      }
    }};
}

} // namespace actors
