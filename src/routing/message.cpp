/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file message.cpp
 */

#include "routing/message.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;
namespace routing {

message::message(size_t id, int source, int destination)
  : id_(id),
    source_(source),
    destination_(std::move(destination)),
    last_weight_(0),
    time_created_(
      duration_cast<milliseconds>(steady_clock::now().time_since_epoch())) {
}

// -- members ----------------------------------------------------------------

size_t message::id() const {
  return id_;
}

int message::source() const {
  return source_;
}

int message::destination() const {
  return destination_;
}

uint64_t message::last_weight() const {
  return last_weight_;
}

const std::vector<int>& message::path() const {
  return path_;
}

std::chrono::milliseconds message::time_created() const {
  return time_created_;
}

// -- public-API -------------------------------------------------------------

void message::update_path(int current_hop) {
  path_.emplace_back(current_hop);
}

void message::update_weight(uint64_t weight) {
  last_weight_ = weight;
}

bool message::path_contains(int node_id) const {
  return std::find(path_.begin(), path_.end(), node_id) != path_.end();
}

} // namespace routing
