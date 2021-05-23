/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file random.cpp
 */

#include "routing/random.hpp"

using namespace caf;

namespace routing {

random::random() {
  // nop
}

random::~random() {
  // nop
}

void random::init(int seed, routing::hyperparameters params) {
  // nop
}

void random::update(const message& msg) {
  // nop
}

void random::delete_route(int node_id) {
  // nop
}

int random::get_route(int dest) {
  return -1;
}

} // namespace routing
