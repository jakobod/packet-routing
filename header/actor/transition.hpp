/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actor {

struct transition_state {
  // Nop
};

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self);
} // namespace actor