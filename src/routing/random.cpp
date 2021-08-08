/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.cpp
 */

#include "routing/random.hpp"

namespace routing {

void random::init(seed_type, hyperparameters) {
  // nop
}

void random::update(const message&) {
  // nop
}

void random::delete_route(id_type) {
  // nop
}

id_type random::get_route(id_type) {
  return -1;
}

} // namespace routing
