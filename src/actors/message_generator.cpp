#include "actors/message_generator.hpp"

namespace actors {

caf::behavior
message_generator_actor(caf::stateful_actor<message_generator_state>* self) {
  // Nop
  return {};
}

} // namespace actors
