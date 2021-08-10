
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

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

namespace benchmark {

behavior benchmarker(stateful_actor<benchmarker_state>* self,
                     [[maybe_unused]] seed_type seed, size_t num_nodes,
                     size_t num_messages, std::string message_log_path,
                     std::string load_log_path) {
  self->set_default_handler(drop);
  self->state.loads.resize(num_nodes);
  return {
    [=](message_delivered_atom, routing::message& msg, bool success) {
      auto now = steady_clock::now().time_since_epoch();
      auto diff = now - msg.time_created();
      auto duration = duration_cast<milliseconds>(diff);
      self->state.results.emplace_back(msg.id(), msg.time_created(),
                                       duration_cast<milliseconds>(now),
                                       std::move(msg.path()), duration,
                                       success);
      if ((self->state.delivered_messages % 100) == 0)
        aout(self) << "[benchmarker] received "
                   << self->state.delivered_messages << std::endl;
      if (++self->state.delivered_messages >= num_messages) {
        std::sort(self->state.results.begin(), self->state.results.end(),
                  [](const auto& a, const auto& b) {
                    return a.msg_id < b.msg_id;
                  });
        self->state.save_messages(message_log_path);
        self->state.save_load(load_log_path);
        aout(self) << "Benchmark done.\nThe benchmark took: "
                   << duration_cast<milliseconds>(
                        self->state.results.back().time_received_
                        - self->state.results.front().time_created_)
                   << std::endl;
        self->quit();
      }
    },
    [=](share_load_atom, load_type current_load, id_type id) {
      self->state.loads.at(id).push_back(current_load);
    },
  };
}

} // namespace benchmark
