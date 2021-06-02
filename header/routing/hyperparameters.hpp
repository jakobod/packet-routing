/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

namespace routing {

struct hyperparameters {
  double pheromone_deposition = 1;
  double pheromone_evaporation = 0.5;
  double alpha = 1;
  double beta = 1;
  float load_weight = 1;
};

} // namespace routing
