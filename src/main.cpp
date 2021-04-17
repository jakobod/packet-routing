/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file main.cpp
 */

#include "caf/all.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct config : caf::actor_system_config {
  config() {
    caf::init_global_meta_objects<caf::id_block::packet_routing>();
    opt_group{custom_options_, "global"};
  }
};

void caf_main(caf::actor_system& sys, const config& args) {
  // nop
}

} // namespace

CAF_MAIN()
