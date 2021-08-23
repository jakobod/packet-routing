/**
 * @author Maximilian Mang
 * @email maximilian.mang@haw-hamburg.de
 * @file benchmarker.hpp
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "benchmark/result.hpp"
#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "routing/message.hpp"
#include "types.hpp"

namespace benchmark {

struct benchmarker_state {
  size_t expected_messages = 0;
  result_set results;
  std::vector<std::vector<id_type>> loads;
  std::string msg_log_path;
  std::string load_log_path;

  /// Adds a received message to the message log
  /// @return `true` if all messages have been received, `false` otherwise
  bool log_message(routing::message& msg, bool success) {
    using namespace std::chrono;
    auto now = steady_clock::now().time_since_epoch();
    auto diff = now - msg.time_created();
    auto duration = duration_cast<milliseconds>(diff);
    results.emplace(msg.id(), msg.time_created(),
                    duration_cast<milliseconds>(now), std::move(msg.path()),
                    duration, success);
    return (results.size() >= expected_messages);
  }

  void save_messages() {
    std::ofstream os(msg_log_path);
    os << "msg_id,time_created,time_received,path_length,duration,arrived"
       << std::endl;
    result_list res(results.begin(), results.end());
    std::sort(res.begin(), res.end(),
              [](const auto& a, const auto& b) { return a.msg_id < b.msg_id; });
    for (const auto& res : res)
      os << res << std::endl;
  }

  void save_load() {
    std::ofstream load_os(load_log_path);
    load_os << "time,";
    for (size_t i = 0; i < loads.size(); ++i)
      load_os << "node_" << std::to_string(i) << ",";
    load_os << std::endl;
    if (loads.empty() || loads.front().empty())
      return;
    size_t y_max = loads.front().size();
    size_t x_max = loads.size();
    for (size_t y = 0; y < y_max; ++y) {
      load_os << 100 * (y + 1) << ",";
      for (size_t x = 0; x < x_max; ++x)
        load_os << loads[x][y] << ",";
      load_os << std::endl;
    }
  }
};

caf::behavior benchmarker(caf::stateful_actor<benchmarker_state>* self,
                          seed_type seed, size_t num_nodes, size_t num_messages,
                          std::string message_log_path,
                          std::string load_log_path);

} // namespace benchmark
