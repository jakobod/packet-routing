/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file entry.hpp
 */

#pragma once

#include <cstdint>

#include "caf/actor.hpp"

namespace routing {

struct entry {
  uint64_t pheromones;
  caf::actor next_hop;
};

} // namespace routing
