/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include <random>

namespace actors {

struct node_state {
  std::vector<caf::actor> transitions;
  std::mt19937 generator;
  uint64_t current_load = 1;
  int node_index;

  void print(caf::stateful_actor<node_state>* actor_stuff, std::string msg);
};

caf::behavior node_actor(caf::stateful_actor<node_state>* self, int index,
                         int seed, caf::actor parent);

} // namespace actors
