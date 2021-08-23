/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file result.hpp
 */

#pragma once

#include <chrono>
#include <ostream>
#include <unordered_set>
#include <vector>

#include "types.hpp"

namespace benchmark {

struct result {
  result(id_type id, std::chrono::milliseconds time_created,
         std::chrono::milliseconds time_received, std::vector<id_type> path,
         std::chrono::milliseconds duration, bool success);

  bool operator==(const result& other) const;

  bool operator!=(const result& other) const;

  friend std::ostream& operator<<(std::ostream& os, const result& x) {
    return os << x.msg_id << "," << x.time_created.count() << ","
              << x.time_received.count() << "," << x.path.size() << ","
              << x.duration.count() << "," << (x.success ? "True" : "False");
  }

  bool success;
  id_type msg_id;
  std::vector<id_type> path;

  std::chrono::milliseconds time_created;
  std::chrono::milliseconds time_received;
  std::chrono::milliseconds duration;
};

} // namespace benchmark

namespace std {

template <>
struct hash<::benchmark::result> {
  size_t operator()(const ::benchmark::result& r) const noexcept {
    auto h1 = hash<bool>{}(r.success);
    auto h2 = hash<size_t>{}(r.msg_id);
    return h1 ^ (h2 << 1);
  }
};

} // namespace std

// -- Type aliases -------------------------------------------------------------

namespace benchmark {

using result_list = std::vector<result>;
using result_set = std::unordered_set<result>;

} // namespace benchmark
