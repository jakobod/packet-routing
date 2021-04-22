/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

struct node_state {
  std::vector<caf::actor> transitions;
  uint64_t current_load = 1;
  int node_index;
};

caf::behavior node_actor(caf::stateful_actor<node_state>* self, caf::actor parent);

} // namespace actors
