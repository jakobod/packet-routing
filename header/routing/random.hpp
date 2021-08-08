/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.hpp
 */

#pragma once

#include "routing/policy.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class random : public policy {
public:
  ~random() override = default;

  void init(seed_type seed, hyperparameters params) override;

  void update(const message& msg) override;

  void delete_route(id_type node_id) override;

  id_type get_route(id_type destination) override;
};

} // namespace routing
