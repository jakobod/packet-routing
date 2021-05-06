/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file main.cpp
 */

#include "caf/all.hpp"

#include "actors/message_generator.hpp"
#include "actors/topology_manager.hpp"
#include "benchmark/benchmarker.hpp"

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
};

void caf_main(actor_system& sys, const config& args) {
  scoped_actor self{sys};
  auto mg = sys.spawn(actors::message_generator, 5000, args.seed);
  auto bm = sys.spawn(benchmark::benchmarker, args.seed);
  auto tm = sys.spawn(actors::topology_manager, mg, bm);
  self->send(tm, generate_atom_v, args.num_nodes, args.num_transitions,
             args.seed);
  std::string dummy;
  std::getline(std::cin, dummy);
}

} // namespace

CAF_MAIN()
