/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file table.hpp
 */

#pragma once

#include <string>
#include <vector>

#include "caf/actor.hpp"

namespace routing {

/// A message that can be routed through the network of nodes.
struct message {
  message(std::string content, caf::actor destination);

  message() = default;

  // -- members ----------------------------------------------------------------

  const std::string& content();

  const caf::actor& destination() const;

  uint64_t path_length();

  const std::vector<int>& path();

  // -- public-API -------------------------------------------------------------

  void update_path(int current_hop);

  void update_weight(uint64_t weight);

  template <class Inspector>
  friend bool inspect(Inspector& f, routing::message& x) {
    return f.object(x).fields(f.field("content", x.content_),
                              f.field("destination", x.destination_),
                              f.field("path", x.path_),
                              f.field("path_length", x.path_length_));
  }

private:
  std::string content_ = "";
  caf::actor destination_;
  std::vector<int> path_;
  uint64_t path_length_ = 0;
};

} // namespace routing
