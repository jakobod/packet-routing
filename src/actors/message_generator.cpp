#include "actors/message_generator.hpp"

#include <random>

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "routing/message.hpp"
#include "type_ids.hpp"

using namespace caf;
using namespace std::chrono;

namespace actors {

behavior message_generator(stateful_actor<message_generator_state>* self,
                           size_t maxWaitTime, size_t seed,
                           size_t num_messages) {
  self->state.gen = std::mt19937(seed);
  self->state.randWaitTime = std::uniform_int_distribution<>(0, maxWaitTime);
  return {
    [=](generate_message_atom) {
      auto& state = self->state;
      if (self->state.nodes.size() >= 2) {
        std::uniform_int_distribution<> randNode(0, state.nodes.size() - 1);
        size_t source = randNode(state.gen);
        size_t destination = randNode(state.gen);
        routing::message msg(self->state.num_messages, source, destination);
        self->send(state.nodes.at(source), message_atom_v, msg);
      }
      milliseconds waitTime(state.randWaitTime(state.gen));
      self->delayed_send(self, waitTime, generate_message_atom_v);
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
