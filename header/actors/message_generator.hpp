/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file message_generator.hpp
 */

#pragma once

#include <cstdint>
#include <random>
#include <vector>

#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "types.hpp"

namespace actors {

struct message_generator_state {
  std::vector<caf::actor> nodes;
  std::mt19937 gen;
  size_t num_messages = 0;

  template <class Node>
  void remove_node(const Node& node) {
    nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
  }
};

caf::behavior
message_generator(caf::stateful_actor<message_generator_state>* self,
                  caf::actor listener, seed_type seed, size_t num_messages,
                  std::chrono::milliseconds drop_timeout);

} // namespace actors
