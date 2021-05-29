#define CAF_TEST_NO_MAIN

#include "caf/test/unit_test_impl.hpp"

#include "caf/init_global_meta_objects.hpp"
#include "type_ids.hpp"

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<id_block::packet_routing>();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}
