/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file entry.hpp
 */

#pragma once

#include <cstdint>

#include "caf/actor.hpp"
#include <cmath>

namespace routing {

struct entry {
  bool operator==(const entry& other) const;
  bool operator!=(const entry& other) const;
  double value() const;
  void update(int taken_path_index);

  double pheromones;
  uint64_t weight;
  int next_hop_index;

  double alpha = 1; // TODO: Let's think about that sometime
  double beta = 1;

  double pheromone_evaporation;
  double pheromone_deposition = 1;
};

} // namespace routing

namespace std {

template <>
struct hash<::routing::entry> {
  std::size_t operator()(const ::routing::entry& e) const noexcept {
    return std::hash<int>{}(e.next_hop_index);
  }
};

} // namespace std