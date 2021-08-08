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

#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "routing/message.hpp"
#include "types.hpp"

namespace benchmark {

struct result {
  result(id_type id, std::chrono::milliseconds time_created,
         std::chrono::milliseconds time_received, std::vector<id_type> path,
         std::chrono::milliseconds duration, bool success)
    : success_(success),
      msg_id(id),
      path(std::move(path)),
      time_created_(time_created),
      time_received_(time_received),
      duration(duration) {
    // nop
  }

  friend std::ostream& operator<<(std::ostream& os, const result& x) {
    std::ostringstream formattedPath;
    if (!x.path.empty()) {
      std::copy(x.path.begin(), x.path.end() - 1,
                std::ostream_iterator<int>(formattedPath, " "));
      formattedPath << !x.path.back();
    }
    return os << x.msg_id << "," << x.time_created_.count() << ","
              << x.time_received_.count() << "," << formattedPath.str() << ","
              << x.duration.count() << "," << (x.success_ ? "True" : "False");
  }

  bool success_;
  id_type msg_id;
  std::vector<id_type> path;

  std::chrono::milliseconds time_created_;
  std::chrono::milliseconds time_received_;
  std::chrono::milliseconds duration;
};

struct benchmarker_state {
  size_t delivered_messages = 0;
  std::vector<result> results;
  std::vector<std::vector<id_type>> loads;
  std::ofstream load_csv;

  void save_load(std::string_view path) {
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
    os << "content,time_created,time_received,path,duration,success"
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
