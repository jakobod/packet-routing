/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright 2011-2020 Jakob Otto                                             *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#pragma once

#include <cstdint>

#include "caf/fwd.hpp"
#include "caf/type_id.hpp"

CAF_BEGIN_TYPE_ID_BLOCK(packet_routing, caf::first_custom_type_id)

  // Every CUSTOM type has to be declared here! CAF messages use
  // CAF_ADD_TYPE_ID(packet_routing, (some_type))

  // Atoms are trait types that are only used for identifying messages. No
  // members whatsoever.
  CAF_ADD_ATOM(packet_routing, hello_world_atom)

CAF_END_TYPE_ID_BLOCK(packet_routing)
