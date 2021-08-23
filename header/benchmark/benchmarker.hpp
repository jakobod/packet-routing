/**
 * @author Maximilian Mang
 * @email maximilian.mang@haw-hamburg.de
 * @file benchmarker.hpp
 */

#pragma once

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
  size_t delivered_messages = 0;
  std::vector<result> results;
  std::vector<std::vector<id_type>> loads;
  std::ofstream load_csv;

  void save_load(const std::string& path) {
    std::ofstream load_os(path);
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

  void save_messages(const std::string& path) {
    std::ofstream os(path);
    os << "msg_id,time_created,time_received,path_length,duration,arrived"
       << std::endl;
    for (const auto& res : results)
      os << res << std::endl;
  }
};

caf::behavior benchmarker(caf::stateful_actor<benchmarker_state>* self,
                          seed_type seed, size_t num_nodes, size_t num_messages,
                          std::string message_log_path,
                          std::string load_log_path);

} // namespace benchmark
