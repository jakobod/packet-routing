#include <gtest/gtest.h>

#include "graph/generator.hpp"

TEST(GraphTest, BasicAssertions) {
  auto g = graph::generate_random_graph(10, 18, 0);
  graph::log_graph(g);
  auto verteces = graph::get_verteces(g);
  ASSERT_EQ(10, verteces.size());
}
