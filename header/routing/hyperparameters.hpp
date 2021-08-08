/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file hyperparameters.hpp
 */

#pragma once

namespace routing {

struct hyperparameters {
  double pheromone_deposition = 1.0;
  double pheromone_evaporation = 0.0;
  double alpha = 1.0;
  double beta = 1.0;
  double load_weight = 1.0;
};

} // namespace routing
