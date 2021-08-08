/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file message.hpp
 */

#pragma once

#include <chrono>
#include <vector>

#include "types.hpp"

namespace routing {

/// A message that can be routed through the network of nodes.
struct message {
  message(id_type id, id_type source, id_type destination);

  message() = default;

  // -- members ----------------------------------------------------------------

  id_type id() const;

  id_type source() const;

  id_type destination() const;

  weight_type last_weight() const;

  std::vector<id_type>& path();

  const std::vector<id_type>& path() const;

  std::chrono::milliseconds time_created() const;

  // -- public-API -------------------------------------------------------------

  void update_path(id_type current_hop);

  void update_weight(weight_type weight);

  bool path_contains(id_type nid) const;

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
  id_type id_ = 0;
  id_type source_ = 0;
  id_type destination_ = 0;

  // Path information
  std::vector<id_type> path_;
  weight_type last_weight_ = 0;
  std::chrono::milliseconds time_created_;
};

} // namespace routing
