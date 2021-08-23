
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
  self->state.expected_messages = num_messages;
  self->state.msg_log_path = message_log_path;
  self->state.load_log_path = load_log_path;
  auto print_progress = [=]() {
    auto delivered_msgs = self->state.results.size();
    if ((delivered_msgs % 1000) == 0)
      aout(self) << "[benchmarker] received " << delivered_msgs << std::endl;
  };
  auto check_done = [=](bool done) {
    if (done) {
      self->state.save_messages();
      self->state.save_load();
      self->quit();
    }
  };
  return {
    [=](message_delivered_atom, routing::message& msg) {
      auto done = self->state.log_message(msg, true);
      print_progress();
      check_done(done);
    },
    [=](message_dropped_atom, routing::message& msg) {
      auto done = self->state.log_message(msg, false);
      print_progress();
      check_done(done);
    },
    [=](share_load_atom, load_type current_load, id_type id) {
      // Disabled until rework is done. Adding new nodes with new node_ids
      // introduces segfaults at this call.
      // aout(self) << "[benchmarker] share
      // load. Access at " << id
      //            << " size() = " << self->state.loads.size() << std::endl;
      // self->state.loads.at(id).push_back(current_load);
    },
  };
}

} // namespace benchmark
