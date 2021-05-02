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
  message(std::string content, int destination, int source);

  message() = default;

  // -- members ----------------------------------------------------------------

  const std::string& content();

  const int destination() const;

  const int source() const;

  uint64_t last_weight();

  const std::vector<int>& path() const;

  // -- public-API -------------------------------------------------------------

  void update_path(int current_hop);

  void update_weight(uint64_t weight);

  template <class Inspector>
  friend bool inspect(Inspector& f, routing::message& x) {
    return f.object(x).fields(f.field("content", x.content_),
                              f.field("destination", x.destination_),
                              f.field("source", x.source_),
                              f.field("path", x.path_),
                              f.field("last_weight", x.last_weight_));
  }

private:
  std::string content_ = "";
  int destination_;
  int source_;
  std::vector<int> path_;
  uint64_t last_weight_ = 0;
};

} // namespace routing
