#include "actor/transition.hpp"

namespace actor {

caf::behavior transition_actor(caf::stateful_actor<transition_state>* self) {
  // Nop
  return {};
}

} // namespace actor