/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include <cstdint>
#include <unordered_map>

#include "caf/actor.hpp"

namespace routing {

struct node_info {
  uint8_t probability;
};

class table {
  std::unordered_map<caf::actor, node_info> transitions;
};

} // namespace routing