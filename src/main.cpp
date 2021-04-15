/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 *
 * main.cpp - Part of the packet routing
 */

#include "caf/all.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

behavior print_actor(event_based_actor* self) {
  return {
    [=](hello_world_atom, size_t num_hellos) {
      for (size_t count = 0; count < num_hellos; ++count)
        aout(self) << "Hello World!" << std::endl;
      self->quit(); // quits the actor after running the complete handler.
    },
  };
}

struct config : caf::actor_system_config {
  config() {
    caf::init_global_meta_objects<caf::id_block::packet_routing>();
    opt_group{custom_options_, "global"}.add(num_hellos, "num_hellos,n",
                                             "number of hellos to print");
  }

  size_t num_hellos = 1;
};

void caf_main(caf::actor_system& sys, const config& args) {
  scoped_actor self{sys};
  auto printer = sys.spawn(print_actor);
  self->send(printer, hello_world_atom_v, args.num_hellos);
}

} // namespace

CAF_MAIN()