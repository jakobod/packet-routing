/**
 * @author Maximilian Mang
 * @email maximilian.mang@haw-hamburg.de
 * @file benchmarker.cpp
 */

#include "benchmark/benchmarker.hpp"

#include <random>

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace benchmark {

behavior benchmarker(stateful_actor<benchmarker_state>* self, size_t,
                     size_t num_messages, std::string output) {
  self->set_default_handler(drop);
  self->state.results.resize(num_messages);
  return {
    [=](message_delivered_atom, routing::message& msg) {
      auto diff = steady_clock::now().time_since_epoch() - msg.time_created();
      auto duration = duration_cast<milliseconds>(diff);
      std::cout << "adding msg.id() = " << msg.id()
                << ", results.size() = " << self->state.results.size()
                << std::endl;
      self->state.results.at(msg.id()) = result{msg.id(), std::move(msg.path()),
                                                duration};
      if (++self->state.delivered_messages >= num_messages) {
        self->state.save_to_file(output);
        self->quit();
      }
    },
  };
}

} // namespace benchmark
