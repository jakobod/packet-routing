#define CAF_TEST_NO_MAIN

#include "caf/test/unit_test_impl.hpp"

#include "caf/init_global_meta_objects.hpp"

int main(int argc, char** argv) {
  using namespace caf;
  core::init_global_meta_objects();
  return test::main(argc, argv);
}
