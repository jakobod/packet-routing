#include "actors/topology_manager.hpp"

#include <boost/graph/graphviz.hpp>
#include <sstream>

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/local_actor.hpp"
#include "caf/stateful_actor.hpp"
#include "type_ids.hpp"
#include "actors/node.hpp"
#include "actors/transition.hpp"

using namespace caf;

namespace actors {

behavior topology_manager_actor(stateful_actor<topology_manager_state>* self) {
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions, int seed) {
      auto graph = graph::generate_random_graph(num_nodes, num_transitions,
                                                seed);

      self->state.graph;
      aout(self) << "[topo] Generated graph:" << std::endl;
      std::stringstream ss;
      boost::write_graphviz(ss, graph);
      aout(self) << ss.str() << std::endl;

      aout(self) << "[topo] Adding nodes" << std::endl;
      for(auto const& node : graph::get_verteces(graph)) {
        self->state.nodes[node] = self->spawn(node_actor);
      }

      aout(self) << "[topo] Adding transitions" << std::endl;
      for(auto const& edge: graph::get_edges(graph)) {
        EdgeIndex index = std::make_pair(std::get<0>(edge), std::get<1>(edge));
        auto node_one = self->state.nodes[index.first];
        auto node_two = self->state.nodes[index.second];
        self->state.transitions[index] = self->spawn(transition_actor, node_one, node_two, self);
      }
    },
    [=](done_atom) {
      topology_manager_state state = self->state;
      state.initialized_transitions++;
      if (state.initialized_transitions >= graph::num_edges(state.graph)) {
        aout(self) << "[topo] Transitions initialized" << std::endl;
      }
    }
  };
}

} // namespace actors
