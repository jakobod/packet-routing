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
#include <vector>

#include "caf/actor.hpp"
#include "caf/behavior.hpp"
#include "caf/stateful_actor.hpp"
#include "routing/message.hpp"

namespace benchmark {

struct result {
  result() = default;
  result(size_t id, std::chrono::milliseconds time_created,
         std::chrono::milliseconds time_received, std::vector<int> path,
         std::chrono::milliseconds duration)
    : msg_id(id),
      time_created_(time_created),
      time_received_(time_received),
      path(std::move(path)),
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
              << x.duration.count();
  }

  size_t msg_id;
  std::chrono::milliseconds time_created_;
  std::chrono::milliseconds time_received_;
  std::vector<int> path;
  std::chrono::milliseconds duration;
};

struct benchmarker_state {
  size_t delivered_messages = 0;
  std::vector<result> results;

  void save_to_file(std::string output) {
    std::ofstream os(output);
    os << "content,time_created,time_received,path,duration" << std::endl;
    for (const auto& res : results)
      os << res << std::endl;
  }
};

caf::behavior benchmarker(caf::stateful_actor<benchmarker_state>* self,
                          size_t seed, size_t num_messages, std::string output);

} // namespace benchmark
