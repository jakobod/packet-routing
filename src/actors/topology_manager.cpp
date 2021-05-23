#include "actors/topology_manager.hpp"

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

behavior topology_manager(stateful_actor<topology_manager_state>* self,
                          actor message_generator, actor listener,
                          routing::hyperparameters params, bool random) {
  self->set_exit_handler([=](const exit_msg&) { self->quit(); });
  self->link_to(listener);
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions, int seed) {
      self->state.graph = graph::generate_random_graph(num_nodes,
                                                       num_transitions, seed);
      auto& g = self->state.graph;
      graph::log_graph(g);
      aout(self) << "[topo] Generated graph. Written to graph.log."
                 << std::endl;
      aout(self) << "[topo] Adding nodes" << std::endl;
      for (const auto& node : graph::get_verteces(g)) {
        auto node_ref = self->spawn(node_actor, node, seed, listener, self,
                                    params, random);
        self->send(message_generator, add_node_atom_v, node_ref);
        self->state.nodes.emplace(node, std::move(node_ref));
      }
      aout(self) << "[topo] Adding transitions" << std::endl;
      for (const auto& e : graph::get_edges(g)) {
        auto node_one = std::make_pair(self->state.nodes.at(e.node_1),
                                       e.node_1);
        auto node_two = std::make_pair(self->state.nodes.at(e.node_2),
                                       e.node_2);
        self->state.transitions.emplace(std::make_pair(e.node_1, e.node_2),
                                        self->spawn(transition_actor, node_one,
                                                    node_two, self, e.weight,
                                                    listener));
      }
      aout(self) << "[topo] Finished building graph with "
                 << graph::num_edges(g) << " edges and "
                 << graph::num_verteces(g) << " verteces." << std::endl;
    },
    [=](done_atom) {
      auto& state = self->state;
      if (++state.initialized_transitions >= graph::num_edges(state.graph)) {
        aout(self) << "[topo] Transitions initialized" << std::endl;
        self->send(message_generator, generate_message_atom_v);
      }
    },
  };
}

} // namespace actors
