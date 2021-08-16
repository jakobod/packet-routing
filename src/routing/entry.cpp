/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file entry.cpp
 */

#include "routing/entry.hpp"

#include <cmath>

namespace routing {

entry::entry(id_type next_hop, hyperparameters params, weight_type weight)
  : pheromones(params.pheromone_deposition),
    weight(weight),
    next_hop(next_hop),
    alpha(params.alpha),
    beta(params.beta),
    pheromone_evaporation(params.pheromone_evaporation),
    pheromone_deposition(params.pheromone_deposition) {
  // nop
}

double entry::value() const {
  return std::pow(pheromones, alpha) * std::pow(weight, beta);
}

void entry::update(weight_type weight) {
  pheromones += pheromone_deposition;
  this->weight = weight;
}

void entry::decay() {
  pheromones = (1 - pheromone_evaporation) * pheromones;
}

bool entry::operator==(const entry& other) const {
  return (next_hop == other.next_hop);
}

bool entry::operator!=(const entry& other) const {
  return !(*this == other);
}

} // namespace routing
