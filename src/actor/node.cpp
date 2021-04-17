#include "actor/node.hpp"

namespace actor {

caf::behavior node_actor(caf::stateful_actor<node_state>* self) {
  // Nop
  return {};
}

} // namespace actor