/**
 * @author Maximilian Mang
 * @email maximilian.mang@haw-hamburg.de
 * @file benchmarker.hpp
 */

#pragma once

#include <cstdint>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>

#include "routing/message.hpp"
#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"

namespace benchmark {

struct benchmarker_state {
    std::ofstream csvFile;
};

caf::behavior
benchmarker(caf::stateful_actor<benchmarker_state>* self, size_t seed);

} // namespace actors
