/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.hpp
 */

#pragma once

#include "routing/policy.hpp"

#include <random>

#include "caf/actor.hpp"
#include "routing/entry.hpp"
#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"

namespace routing {

/// A routing table for routing packets using the ant-routing approach
class random : public policy {
public:
  random();
  ~random() override;

  void init(int seed, hyperparameters params) override;

  void update(const message& msg) override;
  void delete_route(int node_id) override;
  int get_route(int dest) override;
};

} // namespace routing
