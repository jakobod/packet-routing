/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "routing/entry.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class table {
public:
  table();
  ~table();

  void update();

private:
  std::unordered_map<caf::actor, std::vector<entry>> routes;
};

} // namespace routing
