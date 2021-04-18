/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

struct message_generator_state {
  // Nop
};

caf::behavior
message_generator_actor(caf::stateful_actor<message_generator_state>* self);

} // namespace actors