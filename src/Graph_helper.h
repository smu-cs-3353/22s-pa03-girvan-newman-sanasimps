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
    int community = 0;
    bool foundPaths = false;
    bool used = false;
    int distance = -1;
};
struct EdgeProperty {
    std::string Name = "m";
    int count = 0;
};

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty>;
typedef boost::range_detail::integer_iterator<unsigned long> vertexIt;
typedef boost::graph_traits<Graph>::vertex_descriptor vd;



class Graph_helper {
private:
    Graph graph;

public:
    void print_graph();
    void print_edges();
    void print_vertices();

    void read_graphml(const char*);
    void girvan_newman();
    void breadth_first_search(std::map<vd, vd>&, vertexIt);
    void reconstruct_paths(std::map<vd, vd>&, vertexIt);
    void reset_tracking_data(vertexIt);

};

#endif //INC_22S_PA03_GRAPH_HELPER_H
