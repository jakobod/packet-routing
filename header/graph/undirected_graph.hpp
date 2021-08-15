/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file undirected_graph.hpp
 */

#pragma once

#include <optional>
#include <ostream>
#include <sstream>

#include "graph/node.hpp"
#include "graph/transition.hpp"
#include "types.hpp"

namespace graph {

class undirected_graph {
public:
  // -- Constructors -----------------------------------------------------------

  undirected_graph(node_list nodes, transition_list transitions);

  // -- Public API -------------------------------------------------------------

  const node_list& nodes() const;

  const transition_list& transitions() const;

  size_t num_nodes() const;

  size_t num_transitions() const;

  // -- iostream stuff ---------------------------------------------------------

  friend std::ostream& operator<<(std::ostream& os, const undirected_graph& g) {
    std::stringstream ss;
    for (auto node : g.nodes())
      ss << node << std::endl;
    ss << std::endl;
    for (auto trans : g.transitions())
      ss << trans << std::endl;
    return os << ss.str();
  }

private:
  node_list nodes_;

  transition_list transitions_;
};

std::optional<undirected_graph>
generate_random_undirected_graph(size_t num_nodes, size_t num_transitions,
                                 seed_type seed);

} // namespace graph
