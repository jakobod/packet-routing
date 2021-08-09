/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file hyperparameters.hpp
 */

#pragma once

namespace routing {

struct hyperparameters {
  hyperparameters(double pheromone_deposition, double pheromone_evaporation,
                  double alpha, double beta, double load_weight)
    : pheromone_deposition(pheromone_deposition),
      pheromone_evaporation(pheromone_evaporation),
      alpha(alpha),
      beta(beta),
      load_weight(load_weight) {
    // nop
  }

  hyperparameters() = default;

  double pheromone_deposition = 1.0;
  double pheromone_evaporation = 0.0;
  double alpha = 1.0;
  double beta = 1.0;
  double load_weight = 1.0;
};

} // namespace routing
