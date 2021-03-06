/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include "routing/policy.hpp"

#include <unordered_map>
#include <vector>

#include "routing/entry.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class ant : public policy {
  using entry_list = std::vector<entry>;

  using routing_map = std::unordered_map<id_type, entry_list>;

public:
  ~ant() override = default;

  void init(seed_type seed, hyperparameters params) override;

  void add_new_transition(id_type node_id, weight_type weight) override;

  void update(const message& msg) override;

  void delete_route(id_type node_id) override;

  id_type get_route(id_type destination) override;

  routing_map get_routes() {
    return routes_;
  }

private:
  routing_map routes_;
  hyperparameters params_;
};

} // namespace routing
