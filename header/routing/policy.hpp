/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include <iostream>
#include <random>

#include "routing/hyperparameters.hpp"
#include "routing/message.hpp"

namespace routing {

/// An interface for a routing table.
class policy {
public:
  virtual ~policy() = default;

  virtual void init(int, hyperparameters) {
    std::cerr << "init not implemented" << std::endl;
  }

  virtual void update(const message&) {
    std::cerr << "update not implemented" << std::endl;
  }

  virtual void delete_route(int) {
    std::cerr << "delete_route not implemented" << std::endl;
  }

  virtual int get_route(int) {
    std::cerr << "get_route not implemented" << std::endl;
    return -1;
  }

protected:
  std::mt19937 gen;
};

} // namespace routing
