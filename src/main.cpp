/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file main.cpp
 */

#include "caf/all.hpp"

#include <condition_variable>
#include <mutex>

#include "actors/message_generator.hpp"
#include "actors/node.hpp"
#include "actors/topology_manager.hpp"
#include "actors/transition.hpp"
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

  auto tm = sys.spawn(actors::topology_manager_actor);
  self->send(tm, generate_atom_v, args.num_nodes, args.num_transitions,
             args.seed);

  // std::vector<actor> nodes;
  // for (int i = 0; i < args.num_nodes; ++i)
  //  nodes.emplace_back(sys.spawn(actors::node_actor));
  // std::vector<actor> transitions;
  // for (size_t i = 1; i < nodes.size(); ++i)
  //  transitions.emplace_back(sys.spawn(actors::transition_actor,
  //                                     nodes.at(i - 1), nodes.at(i), self,
  //                                     1));

  // We need to wait for the transition to be initialized
  // size_t i = 0;
  // self->receive_for(i, transitions.size())([](done_atom) {});
  // aout(self) << "[main]: initialized" << std::endl;

  //routing::message msg("Hello World!", nodes.back());
  //self->send(nodes.front(), emit_message_atom_v, std::move(msg));

  std::string dummy;
  std::getline(std::cin, dummy);
  //for (const auto& trans : transitions)
  //  self->send_exit(trans, exit_reason::user_shutdown);
}

} // namespace

CAF_MAIN()
