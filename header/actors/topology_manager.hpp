/**
 * @author Henri Burau
 * @email henri.burau@haw-hamburg.de
 * @file topology_manager.hpp
 */

#pragma once

#include <map>

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "graph/undirected_graph.hpp"
#include "routing/hyperparameters.hpp"

#include "types.hpp"

namespace actors {

using transition_id = std::pair<id_type, id_type>;

struct topology_manager_state {
  std::map<transition_id, caf::actor> transitions;
  std::map<id_type, caf::actor> nodes;
  size_t initialized_transitions = 0;
};

caf::behavior
topology_manager(caf::stateful_actor<topology_manager_state>* self,
                 caf::actor message_generator, caf::actor listener,
                 routing::hyperparameters params, bool random);

} // namespace actors
