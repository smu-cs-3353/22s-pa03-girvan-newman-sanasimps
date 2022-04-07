//
// Created by Daniel Ryan on 4/5/22.
//

#ifndef INC_22S_PA03_GRAPH_HELPER_H
#define INC_22S_PA03_GRAPH_HELPER_H

#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <typeinfo>
#include <cxxabi.h>
#include <fstream>
#include <queue>

struct GraphData { int community; };
struct VertexProperty {
    int community;
};
struct EdgeProperty { std::string Name = "m"; };

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty>;

class Graph_helper {
private:
    Graph graph;

public:
    void print_graph();
    void print_edges();
    void print_vertices();

    void read_graphml(const char*);
    void Breadth_first_search();


};

#endif //INC_22S_PA03_GRAPH_HELPER_H
