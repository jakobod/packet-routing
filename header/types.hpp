/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file types.hpp
 */

#pragma once

#include <cstdint>

#include "caf/fwd.hpp"

using id_type = uint64_t;
using seed_type = uint32_t;
using weight_type = uint64_t;
using load_type = uint64_t;

using node_pair = std::pair<caf::actor, id_type>;
using EdgeIndex = std::pair<int, int>;
