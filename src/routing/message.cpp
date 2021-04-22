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

message::message(std::string content, actor destination)
  : content_(std::move(content)),
    destination_(std::move(destination)),
    path_length_(0) {
  // nop
}

// -- members ----------------------------------------------------------------

const std::string& message::content() {
  return content_;
}

const caf::actor& message::destination() const {
  return destination_;
}

uint64_t message::path_length() {
  return path_length_;
}

const std::vector<caf::actor>& message::path() {
  return path_;
}

// -- public-API -------------------------------------------------------------

void message::update_path(caf::actor current_hop) {
  path_.emplace_back(current_hop);
}

void message::update_weight(uint64_t weight) {
  path_length_ += weight;
}

} // namespace routing
