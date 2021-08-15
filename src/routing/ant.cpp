/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file ant.cpp
 */

#include <limits>
#include <numeric>

#include "routing/ant.hpp"
#include "routing/message.hpp"

namespace routing {

void ant::init(seed_type seed, hyperparameters params) {
  this->gen.seed(seed);
  params_ = params;
}

void ant::update(const message& msg) {
  // Update every goal in the hop list
  for (auto index : msg.path()) {
    auto [it, success] = routes_.emplace(index, entry_list());
    auto& entry_l = it->second;
    for (auto& e : entry_l)
      e.decay();
    if (auto to_update = std::find_if(entry_l.begin(), entry_l.end(),
                                      [&](const auto& v) {
                                        return v.next_hop == msg.path().back();
                                      });
        to_update != entry_l.end()) {
      to_update->update(msg.last_weight());
    } else {
      routing::entry e{msg.path().back(), params_};
      e.update(msg.last_weight());
      entry_l.emplace_back(e);
    }
  }
}

void ant::delete_route(id_type node_id) {
  // Delete the destination 'node_id' from the table
  routes_.erase(node_id);
  // Search for all occurrences of `node_id` as next hop and remove them
  for (auto& p : routes_) {
    auto& entries = p.second;
    entries.erase(std::remove_if(entries.begin(), entries.end(),
                                 [=](const auto& e) {
                                   return e.next_hop == node_id;
                                 }),
                  entries.end());
  }
}

id_type ant::get_route(id_type destination) {
  auto it = routes_.find(destination);
  if (it != routes_.end()) {
    auto& entries = it->second;
    auto sum = std::accumulate(entries.begin(), entries.end(), 0.0);
    std::uniform_real_distribution<double> random(0, sum);
    auto randomValue = random(this->gen);
    for (const auto& e : entries) {
      if (randomValue < e.value())
        return e.next_hop;
      randomValue -= e.value();
    }
  }
  return std::numeric_limits<id_type>::max();
}

} // namespace routing
