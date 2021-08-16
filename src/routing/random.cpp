/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.cpp
 */

#include "routing/random.hpp"

#include <limits>

namespace routing {

void random::init(seed_type, hyperparameters) {
  // nop
}

void random::add_new_transition(id_type, weight_type) {
  // nop
}

void random::update(const message&) {
  // nop
}

void random::delete_route(id_type) {
  // nop
}

id_type random::get_route(id_type) {
  return std::numeric_limits<id_type>::max();
}

} // namespace routing
