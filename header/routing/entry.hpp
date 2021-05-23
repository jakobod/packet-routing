/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file entry.hpp
 */

#pragma once

#include <cmath>
#include <cstdint>

#include "caf/actor.hpp"
#include "routing/hyperparameters.hpp"

namespace routing {

struct entry {
  // -- Constructors -----------------------------------------------------------

  entry(int next_hop_index, hyperparameters params);

  // -- public API -------------------------------------------------------------

  double value() const;

  void update(uint64_t weight);

  void decay();

  // -- equality operators -----------------------------------------------------

  bool operator==(const entry& other) const;
  bool operator!=(const entry& other) const;

  // -- public members ---------------------------------------------------------

  double pheromones_ = 0;
  uint64_t weight_ = 0;
  int next_hop_index_ = -1;

  double alpha_ = 1; // TODO: Let's think about that sometime
  double beta_ = 1;

  double pheromone_evaporation_ = 0.01;
  double pheromone_deposition_ = 1;
};

} // namespace routing

namespace std {

template <>
struct hash<::routing::entry> {
  std::size_t operator()(const ::routing::entry& e) const noexcept {
    return std::hash<int>{}(e.next_hop_index_);
  }
};

} // namespace std