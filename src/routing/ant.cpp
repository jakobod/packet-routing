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
  for (const auto& index : msg.path()) {
    auto [it, success] = routes.emplace(index, entry_list());
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

void ant::delete_route([[maybe_unused]] id_type node_id) {
  // for (auto map_it = routes.begin(); map_it != routes.end(); ++map_it) {
  for (const auto& p : routes) {
    for (auto list_it = p.second.begin(); list_it != p.second.end();
         ++list_it) {
      // TODO
      /*if (list_it->next_hop_index == dest) {
        list_it = map_it->second.erase(list_it);
      }*/
    }
  }
}

id_type ant::get_route(id_type destination) {
  auto it = routes.find(destination);
  if (it != routes.end()) {
    auto& entries = it->second;
    auto sum = std::accumulate(entries.begin(), entries.end(), 0.0);
    std::uniform_real_distribution<> random(0, sum);
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
