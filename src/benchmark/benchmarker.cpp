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
  self->state.csvFile.open(output);
  self->state.csvFile << "content,path,duration" << std::endl;
  return {
    [=](message_delivered_atom, routing::message& msg) {
      auto diff = steady_clock::now().time_since_epoch() - msg.time_created();
      auto timePassed = duration_cast<milliseconds>(diff);
      std::ostringstream formattedPath;
      if (!msg.path().empty()) {
        std::copy(msg.path().begin(), msg.path().end() - 1,
                  std::ostream_iterator<int>(formattedPath, " "));
        formattedPath << !msg.path().back();
      }
      self->state.csvFile << msg.content() << "," << formattedPath.str() << ","
                          << timePassed.count() << std::endl;
      if (++self->state.delivered_messages >= num_messages)
        self->quit();
    },
  };
}

} // namespace benchmark
