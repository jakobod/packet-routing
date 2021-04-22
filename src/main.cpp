/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file main.cpp
 */

#include "caf/all.hpp"

#include <chrono>

#include "actors/message_generator.hpp"
#include "actors/topology_manager.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct config : actor_system_config {
  config() {
    init_global_meta_objects<id_block::packet_routing>();
    opt_group{custom_options_, "global"}
      .add(num_nodes, "num-nodes,n", "number of nodes")
      .add(num_transitions, "num-transitions,t", "number of transitions")
      .add(seed, "seed,s", "seed for graph and message generation");
  }

  size_t num_nodes = 1;
  size_t num_transitions = 1;
  int seed = 0;
  std::string actor_name = "";
};

void caf_main(actor_system& sys, const config& args) {
  scoped_actor self{sys};
  auto mg = sys.spawn(actors::message_generator, 10000, args.seed);
  auto tm = sys.spawn(actors::topology_manager_actor, mg);
  self->send(tm, generate_atom_v, args.num_nodes, args.num_transitions,
             args.seed);
  std::string dummy;
  std::getline(std::cin, dummy);
}

} // namespace

CAF_MAIN()
