/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file ant.cpp
 */

#include <iostream>

#include "routing/ant.hpp"
#include "routing/message.hpp"

using namespace caf;

namespace routing {

ant::~ant() {
  // nop
}

void ant::init(int seed, routing::hyperparameters params) {
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
                                        return v.next_hop_index_
                                               == msg.path().back();
                                      });
        to_update != entry_l.end()) {
      to_update->update(msg.last_weight());
    } else {
      routing::entry e{msg.path().back(), params_};
      e.update(msg.last_weight());
      entry_l.emplace_back(std::move(e));
    }
  }
}

void ant::delete_route(int) {
  for (auto map_it = routes.begin(); map_it != routes.end(); ++map_it) {
    for (auto list_it = map_it->second.begin(); list_it != map_it->second.end();
         ++list_it) {
      // TODO
      /*if (list_it->next_hop_index == dest) {
        list_it = map_it->second.erase(list_it);
      }*/
    }
  }
}

int ant::get_route(int dest) {
  double sum_of_values = 0.0;
  auto it = routes.find(dest);
  if (it != routes.end()) {
    auto& entry_l = it->second;
    for (const auto& item : entry_l)
      sum_of_values += item.value();
    std::uniform_real_distribution<> random(0, sum_of_values);
    auto randomValue = random(this->gen);
    for (const auto& e : entry_l) {
      if (randomValue < e.value())
        return e.next_hop_index_;
      randomValue -= e.value();
    }
  }
  return -1;
}

} // namespace routing
