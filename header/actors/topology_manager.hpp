/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include <map>

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "graph/generator.hpp"

namespace actors {

using edge_index = std::pair<int, int>;

struct topology_manager_state {
  std::map<edge_index, caf::actor> transitions;
  std::map<int, caf::actor> nodes;
  size_t initialized_transitions = 0;
  graph::undirected_graph graph;
};

caf::behavior
topology_manager(caf::stateful_actor<topology_manager_state>* self,
                 caf::actor message_generator, caf::actor listener);

} // namespace actors
