/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include <random>

#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"

namespace routing {

/// An interface for a routing table.
class policy {
public:
  policy() = default;
  virtual ~policy() = default;

  virtual void init(int seed, hyperparameters params) = 0;

  virtual void update(const message& msg) = 0;
  virtual void delete_route(int node_id) = 0;
  virtual int get_route(int dest) = 0;

protected:
  std::mt19937 gen;
};

} // namespace routing
