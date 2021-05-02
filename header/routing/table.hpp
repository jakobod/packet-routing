/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include <random>
#include <unordered_map>
#include <vector>

#include "caf/actor.hpp"
#include "routing/entry.hpp"
#include "routing/message.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class table {
  using entry_list = std::vector<entry>;
  using routing_map = std::unordered_map<int, entry_list>;

public:
  table();
  ~table();

  void init(int seed);

  void update(const message& msg);
  void delete_route(int node_id);
  int get_route(int dest);

private:
  routing_map routes;
  std::mt19937 gen;
};

} // namespace routing
