#define CAF_SUITE routing.message

#include "caf/test/dsl.hpp"

#include "routing/message.hpp"

namespace {

struct fixture {
  fixture() : msg{1, 2, 3} {
    // nop
  }

  routing::message msg;
};

} // namespace

CAF_TEST_FIXTURE_SCOPE(node_tests, fixture)

CAF_TEST(simple initialization) {
  CAF_CHECK_EQUAL(msg.id(), id_type{1});
  CAF_CHECK_EQUAL(msg.source(), id_type{2});
  CAF_CHECK_EQUAL(msg.destination(), id_type{3});
  CAF_CHECK_EQUAL(msg.last_weight(), weight_type{1});
  CAF_CHECK_EQUAL(msg.path().size(), size_t{0});
}

CAF_TEST(update path) {
  for (id_type i = 0; i < 10; ++i)
    msg.update_path(i);
  for (id_type i = 0; i < 10; ++i)
    CAF_CHECK_EQUAL(msg.path_contains(i),
                    std::find(msg.path().begin(), msg.path().end(), i)
                      != msg.path().end());
}

CAF_TEST(update weight) {
  for (weight_type w = 0; w < 10; ++w) {
    msg.update_weight(w);
    CAF_CHECK_EQUAL(msg.last_weight(), w);
  }
}

CAF_TEST_FIXTURE_SCOPE_END()
