/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.cpp
 */

#include "routing/random.hpp"

namespace routing {

random::random() {
  // nop
}

random::~random() {
  // nop
}

void random::init(int, hyperparameters) {
  // nop
}

void random::update(const message&) {
  // nop
}

void random::delete_route(int) {
  // nop
}

int random::get_route(int) {
  return -1;
}

} // namespace routing
