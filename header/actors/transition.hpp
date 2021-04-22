/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

using ActorPair = std::pair<caf::actor, int>;
using EdgeIndex = std::pair<int, int>;

struct transition_state {
  uint64_t weight;
  EdgeIndex index;
};

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self,
                               caf::actor node_one, int node_one_index,
                               caf::actor node_two, int node_two_index,
                               caf::actor parent, int weight);

} // namespace actors
