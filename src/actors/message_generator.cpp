#include "actors/message_generator.hpp"

#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;

using distribution_type = std::uniform_int_distribution<>;
using dist_result_type = distribution_type::result_type;

namespace actors {

behavior message_generator(stateful_actor<message_generator_state>* self,
                           seed_type seed, size_t num_messages) {
  self->state.gen.seed(seed);
  return {
    [=](generate_message_atom) {
      auto& state = self->state;
      if (state.nodes.size() >= 2) {
        distribution_type dist(0, static_cast<dist_result_type>(
                                    state.nodes.size() - 1));
        auto source = static_cast<int>(dist(state.gen));
        auto destination = static_cast<int>(dist(state.gen));
        routing::message msg(self->state.num_messages, source, destination);
        self->send(state.nodes.at(source), message_atom_v, msg);
      }
      self->send(self, generate_message_atom_v);
      if (++self->state.num_messages >= num_messages)
        self->quit();
    },
    [=](remove_node_atom, const actor& node) {
      self->state.nodes.erase(std::remove(self->state.nodes.begin(),
                                          self->state.nodes.end(), node),
                              self->state.nodes.end());
    },
    [=](add_node_atom, const actor& node) {
      self->state.nodes.push_back(node);
    },
  };
}

} // namespace actors
