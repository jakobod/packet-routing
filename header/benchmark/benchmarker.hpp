/**
 * @author Maximilian Mang
 * @email maximilian.mang@haw-hamburg.de
 * @file benchmarker.hpp
 */

#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "routing/message.hpp"

namespace benchmark {

struct benchmarker_state {
  std::ofstream csvFile;
  size_t delivered_messages = 0;
};

caf::behavior benchmarker(caf::stateful_actor<benchmarker_state>* self,
                          size_t seed, size_t num_messages);

} // namespace benchmark
