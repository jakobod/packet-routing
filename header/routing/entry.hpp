/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file entry.hpp
 */

#pragma once

#include <functional>

#include "routing/hyperparameters.hpp"
#include "types.hpp"

namespace routing {

struct entry {
  // -- Constructors -----------------------------------------------------------

  entry(id_type next_hop, hyperparameters params);

  // -- public API -------------------------------------------------------------

  double value() const;

  void update(weight_type weight);

  void decay();

  // -- operators --------------------------------------------------------------

  bool operator==(const entry& other) const;

  bool operator!=(const entry& other) const;

  operator double() const {
    return value();
  }

  // -- public members ---------------------------------------------------------

  double pheromones = 0;
  weight_type weight = 0;
  id_type next_hop = -1;

  double alpha = 1;
  double beta = 1;

  double pheromone_evaporation = 0;
  double pheromone_deposition = 1;
};

} // namespace routing

namespace std {

template <>
struct hash<::routing::entry> {
  std::size_t operator()(const ::routing::entry& e) const noexcept {
    return std::hash<id_type>{}(e.next_hop);
  }
};

} // namespace std
