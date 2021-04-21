/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include <vector>
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include <random>

namespace actors {

struct message_generator_state {
  std::vector<caf::actor> nodes;
  std::uniform_int_distribution<> randWaitTime;
  std::mt19937 gen; //Standard mersenne_twister_engine seeded with rd()
};

caf::behavior
message_generator_actor(caf::stateful_actor<message_generator_state>* self);

} // namespace actors 