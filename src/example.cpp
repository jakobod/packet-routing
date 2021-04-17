/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file example.cpp
 */

#include "caf/all.hpp"

#include "type_ids.hpp"

using namespace caf;

namespace {

// -- stateful-actor -----------------------------------------------------------

// State of the teller actor. Can contain any members and even functions!
struct state {
  size_t num_calls = 0;

  // Does the same as the message handler, but may be cleaner.
  void tell(stateful_actor<state>* self) {
    ++num_calls;
    aout(self) << "Function: I've been called " << num_calls << " times!"
               << std::endl;
  }

};

behavior teller_actor(stateful_actor<state>* self) {
  return {
    [=](tell_atom) {
      // Counts the number of calls in its state..
      ++self->state.num_calls;
      // ..and prints it.
      aout(self) << "Handler: I've been called " << self->state.num_calls
                 << " times!" << std::endl;
    },
    [=](tell_func_atom) {
      // Calls the function from the state.
      self->state.tell(self);
    },
    
  };
}

// -- event-based-actor --------------------------------------------------------

behavior print_actor(event_based_actor* self) {
  return {
    [=](hello_world_atom, size_t num_hellos) {
      for (size_t count = 0; count < num_hellos; ++count)
        aout(self) << "Hello World!" << std::endl;
      self->quit();
    },
  };
}

struct config : caf::actor_system_config {
  config() {
    caf::init_global_meta_objects<caf::id_block::packet_routing>();
    opt_group{custom_options_, "global"}
      .add(num_hellos, "num-hellos,n", "number of hellos to print")
      .add(actor_name, "actor-name,a",
           "The name of the actor to spawn. Can be: [teller|printer]");
  }

  size_t num_hellos = 1;
  std::string actor_name = "";
};

void caf_main(caf::actor_system& sys, const config& args) {
  // Scoped actor does not run. Is only used for 'manual' actions like sending
  // messages to actors.
  scoped_actor self{sys};
  if (args.actor_name == "printer") {
    // Create and run an actor.
    auto printer = sys.spawn(print_actor);
    // Let `printer` print some 'Hello World!'s.
    self->send(printer, hello_world_atom_v, args.num_hellos);
  } else if (args.actor_name == "teller") {
    // Create and run an actor.
    auto teller = sys.spawn(teller_actor);
    // Send the actor some messages with different atoms.
    self->send(teller, tell_atom_v);
    self->send(teller, tell_atom_v);
    self->send(teller, tell_func_atom_v);
    self->send(teller, tell_func_atom_v);
  } else {
    std::cerr << "no actor \"" << args.actor_name << "\" known" << std::endl;
  }
}

} // namespace

CAF_MAIN()
