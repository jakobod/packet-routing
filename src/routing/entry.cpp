#include "routing/entry.hpp"

namespace routing {
double entry::value() {
  return std::pow(pheromones, alpha) * std::pow(weight, beta);
}

void entry::update(int taken_path_index) {
  pheromones = (1 - pheromone_evaporation) * pheromones;

  if (taken_path_index == next_hop_index) {
    pheromones += pheromone_deposition;
  }
}

} // namespace routing
