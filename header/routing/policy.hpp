/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file policy.hpp
 */

#pragma once

#include <random>

#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"
#include "types.hpp"

namespace routing {

/// An interface for a routing table.
class policy {
public:
  // -- constructors, destructors ----------------------------------------------

  virtual ~policy() = default;

  // -- initialization ---------------------------------------------------------

  virtual void init(seed_type seed, hyperparameters params) = 0;

  // -- public API -------------------------------------------------------------

  virtual void add_new_transition(id_type node_id, weight_type weight) = 0;

  virtual void update(const message& msg) = 0;

  virtual void delete_route(id_type node_id) = 0;

  virtual id_type get_route(id_type destination) = 0;

protected:
  std::mt19937 gen;
};

} // namespace routing
