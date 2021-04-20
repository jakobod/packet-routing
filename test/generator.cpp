#include <gtest/gtest.h>
#include <boost/graph/graphviz.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

#include "graph/generator.hpp"

TEST(GraphTest, BasicAssertions) {
    auto graph = graph::generate_random_graph(10, 18, 0);
    std::ofstream graph_log;
    graph_log.open ("graph.log");

    boost::write_graphviz(graph_log, graph);
    graph_log.close();

    auto verteces = graph::get_verteces(graph);
    ASSERT_EQ(10, verteces.size());
}