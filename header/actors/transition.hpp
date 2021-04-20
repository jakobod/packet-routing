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
  uint64_t weight;
};

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self,
                               caf::actor node_one, caf::actor node_two,
                               caf::actor parent, uint64_t weight);

} // namespace actors
