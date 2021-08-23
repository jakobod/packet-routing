/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include <memory>
#include <random>

#include "caf/behavior.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/stateful_actor.hpp"
#include "routing/hyperparameters.hpp"
#include "routing/policy.hpp"
#include "types.hpp"

namespace actors {

struct node_state {
  id_type node_id = std::numeric_limits<id_type>::max();
  size_t message_count = 0;
  load_type current_load = 0;
  double load_weight = 0.0;
  std::mt19937 generator;
  std::vector<node_pair> transitions;
  std::shared_ptr<routing::policy> routing_table;

  void remove_transition(const caf::actor_addr& source) {
    auto node_id = from_act(source);
    routing_table->delete_route(node_id);
    transitions.erase(std::remove_if(transitions.begin(), transitions.end(),
                                     [=](const auto& p) {
                                       return p.first == source;
                                     }),
                      transitions.end());
  }

  caf::actor from_id(id_type index) {
    if (auto it
        = std::find_if(transitions.begin(), transitions.end(),
                       [=](const auto& p) { return p.second == index; });
        it != transitions.end())
      return it->first;
    return nullptr;
  }

  template <class Actor>
  id_type from_act(Actor& act) {
    if (auto it = std::find_if(transitions.begin(), transitions.end(),
                               [=](const auto& p) { return p.first == act; });
        it != transitions.end())
      return it->second;
    return -1;
  }

  caf::actor pick_random() {
    std::uniform_int_distribution<id_type> distrib(0, transitions.size() - 1);
    return transitions.at(distrib(generator)).first;
  }
};

caf::behavior node(caf::stateful_actor<node_state>* self, id_type node_id,
                   seed_type seed, caf::actor listener, caf::actor parent,
                   routing::hyperparameters params, bool random);

} // namespace actors
