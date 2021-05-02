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

namespace routing {

message::message(std::string content, int destination, int source)
  : content_(std::move(content)),
    destination_(std::move(destination)),
    last_weight_(0) {
  // nop
}

// -- members ----------------------------------------------------------------

const std::string& message::content() const {
  return content_;
}

const int message::destination() const {
  return destination_;
}

const int message::source() const {
  return source_;
}

uint64_t message::last_weight() const {
  return last_weight_;
}

const std::vector<int>& message::path() const {
  return path_;
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
