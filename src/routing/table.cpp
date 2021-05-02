/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.cpp
 */

#include "routing/table.hpp"
#include "routing/message.hpp"

using namespace caf;

namespace routing {

table::table(int seed) : gen(seed) {
  // nop
}

table::~table() {
  // nop
}

void table::update(const routing::message& msg) {
  // Update every goal in the hop list
  for (const auto& index : msg.path()) {
    auto pair = routes.emplace(index, entry_list());

    for (auto& list_item : pair.first->second) {
      list_item.update(msg.path().at(msg.path().size() - 1));
    }
  }
}

void table::delete_route(const caf::actor& dest) {
  for (auto map_it = routes.begin(); map_it != routes.end(); ++map_it) {
    for (auto list_it = map_it->second.begin(); list_it != map_it->second.end();
         ++list_it) {
      if (list_it->next_hop == dest) {
        list_it = map_it->second.erase(list_it);
      }
    }
  }
}

caf::actor table::get_route(int dest) {
  double sum_of_values;

  auto& list = routes.at(dest);
  for (const auto& item : list) {
    sum_of_values += item.value();
  }

  std::uniform_real_distribution<> random(0, sum_of_values);
  double randomValue = random(gen);

  for (const auto& item : list) {
    if (randomValue < item.value()) {
      return item.next_hop;
    }
    randomValue -= item.value();
  }

  return nullptr;
}

} // namespace routing
