/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "types.hpp"

namespace actors {

struct transition_state {
  weight_type weight = 0;
  size_t received_dones = 0;
};

caf::behavior transition(caf::stateful_actor<transition_state>* self,
                         node_pair node_1, node_pair node_2, caf::actor parent,
                         weight_type weight, caf::actor benchmarker,
                         bool alive);

} // namespace actors
