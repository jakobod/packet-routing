/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file main.cpp
 */

#include "caf/all.hpp"

#include "actors/message_generator.hpp"
#include "actors/topology_manager.hpp"
#include "benchmark/benchmarker.hpp"
#include "routing/hyperparameters.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

struct config : actor_system_config {
  config() {
    init_global_meta_objects<id_block::packet_routing>();
    opt_group{custom_options_, "global"}
      .add(num_messages, "messages,m", "number of messages to route")
      .add(random, "random,r", "Route randomly")
      .add(output, "output,o", "The output name of the resulting csv")
      .add(num_nodes, "num-nodes,n", "number of nodes")
      .add(num_transitions, "num-transitions,t", "number of transitions")
      .add(seed, "seed,s", "seed for graph and message generation")
      .add(pheromone_deposition, "deposition,d",
           "Evaporation coefficient of pheromones")
      .add(pheromone_evaporation, "evaporation,e",
           "Evaporation coefficient of pheromones")
      .add(alpha, "alpha,a", "Controls the influence of the pheromones")
      .add(beta, "beta,b", "Controls the influence of the path weight")
      .add(load_weight, "load_weight,w", "Controls Weight of load")
      .add(log_graph, "log,l", "Writes the generated graph to a file");
  }

  // Benchmark
  size_t num_messages = 1000;
  bool random = false;
  std::string output = "results.csv";
  bool log_graph = false;
  // Graph generation
  size_t num_nodes = 1;
  size_t num_transitions = 1;
  int seed = 0;
  // Hyperparameters (learning)
  double pheromone_deposition = 1;
  double pheromone_evaporation = 0.5;
  double alpha = 1;
  double beta = 1;
  float load_weight = 1;
};

void caf_main(actor_system& sys, const config& args) {
  scoped_actor self{sys};
  auto mg = sys.spawn(actors::message_generator, 10, args.seed,
                      args.num_messages);
  auto bm = sys.spawn(benchmark::benchmarker, args.seed, args.num_messages,
                      args.output);
  auto tm = sys.spawn(actors::topology_manager, mg, bm,
                      routing::hyperparameters{args.pheromone_deposition,
                                               args.pheromone_evaporation,
                                               args.alpha, args.beta, args.load_weight},
                      args.random, args.log_graph);
  self->send(tm, generate_atom_v, args.num_nodes, args.num_transitions,
             args.seed);
}

} // namespace

CAF_MAIN()
