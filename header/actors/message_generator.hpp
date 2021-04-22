/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include <vector>

#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace actors {

struct message_generator_state {
  std::vector<caf::actor> nodes;
  std::uniform_int_distribution<> randWaitTime;
  std::mt19937 gen;
};

caf::behavior
message_generator(caf::stateful_actor<message_generator_state>* self,
                  size_t maxWaitTime, size_t seed);

} // namespace actors
