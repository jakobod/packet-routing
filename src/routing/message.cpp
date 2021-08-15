/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file message.cpp
 */

#include "routing/message.hpp"

#include <algorithm>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

namespace routing {

message::message(id_type id, id_type source, id_type destination)
  : id_(id),
    source_(source),
    destination_(destination),
    last_weight_(1),
    time_created_(
      duration_cast<milliseconds>(steady_clock::now().time_since_epoch())) {
}

// -- members ----------------------------------------------------------------

id_type message::id() const {
  return id_;
}

id_type message::source() const {
  return source_;
}

id_type message::destination() const {
  return destination_;
}

weight_type message::last_weight() const {
  return last_weight_;
}

std::vector<id_type>& message::path() {
  return path_;
}

const std::vector<id_type>& message::path() const {
  return path_;
}

std::chrono::milliseconds message::time_created() const {
  return time_created_;
}

// -- public-API -------------------------------------------------------------

void message::update_path(id_type current_hop) {
  path_.emplace_back(current_hop);
}

void message::update_weight(weight_type weight) {
  last_weight_ = weight;
}

bool message::path_contains(id_type node_id) const {
  return std::find(path_.begin(), path_.end(), node_id) != path_.end();
}

} // namespace routing
