/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include "routing/policy.hpp"

#include <random>
#include <unordered_map>
#include <vector>

#include "caf/actor.hpp"
#include "routing/entry.hpp"
#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class ant : public policy {
  using entry_list = std::vector<entry>;
  using routing_map = std::unordered_map<int, entry_list>;

public:
  ant();
  ~ant() override;

  void init(int seed, hyperparameters params) override;

  void update(const message& msg) override;
  void delete_route(int node_id) override;
  int get_route(int dest) override;

private:
  routing_map routes;
  hyperparameters params_;
};

} // namespace routing
