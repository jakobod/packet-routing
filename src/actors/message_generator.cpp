#include "actors/message_generator.hpp"

#include "caf/actor_ostream.hpp"
#include "caf/event_based_actor.hpp"
#include "caf/fwd.hpp"
#include "type_ids.hpp"
#include "routing/message.hpp"
#include <random>

namespace actors {

caf::behavior
message_generator_actor(caf::stateful_actor<message_generator_state>* self,
                        size_t maxWaitTime, size_t seed) {
  self->state.gen = std::mt19937(seed);
  self->state.randWaitTime = std::uniform_int_distribution<>(0,
                                                             maxWaitTime - 1);
  using namespace std::literals::chrono_literals;
  self->delayed_send(
    self, std::chrono::milliseconds(self->state.randWaitTime(self->state.gen)),
    generate_message_atom_v);
  return {[=](generate_message_atom) {
            if (self->state.nodes.size() > 0) {
              std::uniform_int_distribution<> randNode(
                0, self->state.nodes.size() - 1);
              size_t receiveIndex = randNode(self->state.gen);
              size_t sendIndex = randNode(self->state.gen);
              routing::message msg("Hello", self->state.nodes[receiveIndex]);
              self->send(self->state.nodes[sendIndex], message_atom_v, msg);
              aout(self) << "[Message Generator]: Generating new message from "
                         << sendIndex << " to " << receiveIndex << " out of " << self->state.nodes.size()
                         << " Nodes" << std::endl;
            }
            std::chrono::milliseconds waitTime = std::chrono::milliseconds(
              self->state.randWaitTime(self->state.gen));
            self->delayed_send(self, waitTime, generate_message_atom_v);
            aout(self) << "[Message Generator]: Next Message in " << waitTime
                       << std::endl;
          },
          [=](remove_node_atom, const caf::actor& node) {
            aout(self) << "[Message Generator]: removing Node" << std::endl;
            self->state.nodes.erase(std::remove(self->state.nodes.begin(),
                                                self->state.nodes.end(), node),
                                    self->state.nodes.end());
          },
          [=](add_node_atom, const caf::actor& node) {
            aout(self) << "[Message Generator]: adding Node" << std::endl;
            self->state.nodes.push_back(node);
          }};
}
} // namespace actors
