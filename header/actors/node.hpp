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
  // Nop
  std::vector<caf::actor> transitions;
};

caf::behavior node_actor(caf::stateful_actor<node_state>* self);

} // namespace actors