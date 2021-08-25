
/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file result.cpp
 */

#include "benchmark/result.hpp"

using namespace caf;

namespace benchmark {

result::result(id_type id, std::chrono::milliseconds time_created,
               std::chrono::milliseconds time_received,
               std::vector<id_type> path, std::chrono::milliseconds duration,
               bool success)
  : success(success),
    msg_id(id),
    path(std::move(path)),
    time_created(time_created),
    time_received(time_received),
    duration(duration) {
  // nop
}

bool result::operator==(const result& other) const {
  return (this->msg_id == other.msg_id)
         && (this->time_created == other.time_created);
}

bool result::operator!=(const result& other) const {
  return !(*this == other);
}

} // namespace benchmark
