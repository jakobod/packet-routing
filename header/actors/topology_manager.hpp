/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "graph/generator.hpp"

namespace actors {

typedef std::pair<int, int> EdgeIndex;

struct topology_manager_state {
  std::map<EdgeIndex, caf::actor> transitions;
  std::map<int, caf::actor> nodes;

  int initialized_transitions;

  graph::Graph graph;
};

caf::behavior
topology_manager_actor(caf::stateful_actor<topology_manager_state>* self);

} // namespace actors
