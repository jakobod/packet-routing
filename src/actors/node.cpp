#include "actors/node.hpp"

#include "caf/all.hpp"
#include "type_ids.hpp"

using namespace caf;

namespace actors {

behavior node_actor(stateful_actor<node_state>* self) {
  self->set_down_handler([=](const down_msg& msg) {
    aout(self) << "down: transition " << msg.source << " down. Reason "
               << msg.reason << std::endl;
    auto& transitions = self->state.transitions;
    transitions.erase(std::remove_if(begin(transitions), end(transitions),
                                     [=](const auto& x) {
                                       return x == msg.source;
                                     }),
                      end(transitions));
  });
  return {[=](register_transition_atom, actor trans) {
            aout(self) << "[node]: Got new transition " << trans << std::endl;
            self->state.transitions.push_back(trans);
            self->monitor(trans);
            return done_atom_v;
          },
          [=](message_atom, const std::string& payload) {
            aout(self) << "[node]: got message: " << payload << std::endl;
          },
          [=](emit_message_atom, const std::string& payload) {
            aout(self) << "[node]: emitting message" << std::endl;
            // TODO: Make real stuff happening
            for (const auto& transition : self->state.transitions) {
              self->send(transition, message_atom_v, payload);
            }
          }};
}

} // namespace actors
