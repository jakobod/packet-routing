/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file message.hpp
 */

#pragma once

#include "caf/actor.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace routing {

/// A message that can be routed through the network of nodes.
struct message {
  message(size_t id, int source, int destination);

  message() = default;

  // -- members ----------------------------------------------------------------

  size_t id() const;

  int source() const;

  int destination() const;

  uint64_t last_weight() const;

  const std::vector<int>& path() const;

  std::chrono::milliseconds time_created() const;

  // -- public-API -------------------------------------------------------------

  void update_path(int current_hop);

  void update_weight(uint64_t weight);

  bool path_contains(int node_id) const;

  // -- CAF inspection function ------------------------------------------------

  template <class Inspector>
  friend bool inspect(Inspector& f, routing::message& x) {
    return f.object(x).fields(f.field("id", x.id_),
                              f.field("source", x.source_),
                              f.field("destination", x.destination_),
                              f.field("path", x.path_),
                              f.field("last_weight", x.last_weight_),
                              f.field("time_created", x.time_created_));
  }

private:
  // Addressing
  size_t id_;
  int source_;
  int destination_;

  // Path information
  std::vector<int> path_;
  uint64_t last_weight_ = 0;
  std::chrono::milliseconds time_created_;
};

} // namespace routing
