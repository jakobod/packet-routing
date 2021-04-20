#include "actors/topology_manager.hpp"

#include <boost/graph/graphviz.hpp>
#include <sstream>

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "caf/local_actor.hpp"
#include "caf/stateful_actor.hpp"
#include "graph/generator.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior topology_manager_actor(stateful_actor<topology_manager_state>* self) {
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions, int seed) {
      auto graph = graph::generate_random_graph(num_nodes, num_transitions,
                                                seed);
      aout(self) << "Generated graph:" << std::endl;
      std::stringstream ss;
      boost::write_graphviz(ss, graph);
      aout(self) << ss.str() << std::endl;
    },
  };
}

} // namespace actors
