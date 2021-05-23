#include "routing/entry.hpp"

#include <iostream>

namespace routing {

// -- Constructors -------------------------------------------------------------

entry::entry(int next_hop_index, hyperparameters params)
  : next_hop_index_(next_hop_index),
    alpha_(params.alpha),
    beta_(params.beta),
    pheromone_evaporation_(params.pheromone_evaporation),
    pheromone_deposition_(params.pheromone_deposition) {
  // nop
}

// -- public API ---------------------------------------------------------------

double entry::value() const {
  return std::pow(pheromones_, alpha_) * std::pow(weight_, beta_);
}

void entry::update(uint64_t weight) {
  pheromones_ += pheromone_deposition_;
  weight_ = weight;
}

void entry::decay() {
  pheromones_ = (1 - pheromone_evaporation_) * pheromones_;
}

// -- equality operators -------------------------------------------------------

bool entry::operator==(const entry& other) const {
  return (next_hop_index_ == other.next_hop_index_);
}

bool entry::operator!=(const entry& other) const {
  return !(*this == other);
}

} // namespace routing
