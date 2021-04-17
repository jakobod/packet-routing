/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actor {

struct node_state {
  // Nop
};

caf::behavior node_actor(caf::stateful_actor<node_state>* self);
} // namespace actor