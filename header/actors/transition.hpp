/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

struct transition_state {
  caf::actor node_one;
  caf::actor node_two;
};

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self,
                               caf::actor node_one, caf::actor node_two);

} // namespace actors