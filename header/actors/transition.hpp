/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

using node_pair = std::pair<caf::actor, int>;
using EdgeIndex = std::pair<int, int>;

struct transition_state {
  uint64_t weight;
  EdgeIndex index;
  size_t received_dones = 0;
};

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self,
                               node_pair node_1, node_pair node_2,
                               caf::actor parent, int weight);

} // namespace actors
