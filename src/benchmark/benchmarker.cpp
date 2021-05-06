#include "benchmark/benchmarker.hpp"

#include <random>

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace benchmark {

behavior benchmarker(stateful_actor<benchmarker_state>* self, size_t seed) {
  self->set_default_handler(drop);
  return {
    [=](message_delivered_atom, routing::message& msg) {
      aout(self) << "[node " << msg.destination()
                   << "]: Got message: " << msg.content() << msg.path()
                   << std::endl;
    },
  };
}

} // namespace actors
