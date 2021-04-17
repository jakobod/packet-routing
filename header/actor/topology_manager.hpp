/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actor {

struct topology_manager_state {
  // Nop
};

caf::behavior
topology_manager_actor(caf::stateful_actor<topology_manager_state>* self);
} // namespace actor