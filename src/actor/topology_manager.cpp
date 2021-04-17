#include "actor/topology_manager.hpp"
#include "caf/actor_ostream.hpp"
#include "caf/all.hpp"
#include "caf/stateful_actor.hpp"
#include "graph_generator.hpp"
#include "type_ids.hpp"
#include <boost/graph/graphviz.hpp>
#include <sstream>

using namespace caf;

namespace actor {

behavior topology_manager_actor(stateful_actor<topology_manager_state>* self) {
  return {
    [=](generate_atom, size_t num_nodes, size_t num_transitions, int seed) {
      auto graph = generate_random_graph(num_nodes, num_transitions, seed);
      aout(self) << "Generated graph:" << std::endl;

      std::stringstream ss;
      boost::write_graphviz(ss, graph);
      aout(self) << ss.str() << std::endl;
    },
  };
}

} // namespace actor