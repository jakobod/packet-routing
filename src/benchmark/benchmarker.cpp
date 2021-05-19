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
  self->state.csvFile.open ("benchmark.csv");
  self->state.csvFile <<  "content, path, duration" << std::endl;
  return {
    [=](message_delivered_atom, routing::message& msg) {
      auto timePassed =  std::chrono::duration_cast<std::chrono::microseconds>(steady_clock::now().time_since_epoch() - msg.time_created());
      aout(self) << "[node " << msg.destination()
                   << "]: Got message: " << msg.content() << msg.path() << " Duration: " << timePassed.count() << "us last Weight: " << msg.last_weight()
                   << std::endl;
      
      std::ostringstream formattedPath;
  
      if (!msg.path().empty()) {
        std::copy(msg.path().begin(), msg.path().end()-1,
        std::ostream_iterator<int>(formattedPath, " "));
        formattedPath << !msg.path().back();
      }
      self->state.csvFile << msg.content() << "," << formattedPath.str() << "," <<  timePassed.count() << std::endl;
    },
  };
}

} // namespace actors
