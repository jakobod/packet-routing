/**
 * @author Jakob Otto
 * @email jakob.otto@haw-hamburg.de
 * @file type_ids.hpp
 */

#pragma once

#include "caf/type_id.hpp"

#include "routing/message.hpp"

CAF_BEGIN_TYPE_ID_BLOCK(packet_routing, caf::first_custom_type_id)

  // Every CUSTOM type has to be declared here! CAF messages use
  CAF_ADD_TYPE_ID(packet_routing, (routing::message))

  // Atoms are trait types that are only used for identifying messages.
  CAF_ADD_ATOM(packet_routing, tell_atom)
  CAF_ADD_ATOM(packet_routing, tell_func_atom)
  CAF_ADD_ATOM(packet_routing, hello_world_atom)

  CAF_ADD_ATOM(packet_routing, init_atom)
  CAF_ADD_ATOM(packet_routing, done_atom)
  CAF_ADD_ATOM(packet_routing, generate_atom)
  CAF_ADD_ATOM(packet_routing, register_transition_atom)
  CAF_ADD_ATOM(packet_routing, message_atom)
  CAF_ADD_ATOM(packet_routing, emit_message_atom)
  CAF_ADD_ATOM(packet_routing, generate_message_atom)
  CAF_ADD_ATOM(packet_routing, add_node_atom)
  CAF_ADD_ATOM(packet_routing, remove_node_atom)

CAF_END_TYPE_ID_BLOCK(packet_routing)
