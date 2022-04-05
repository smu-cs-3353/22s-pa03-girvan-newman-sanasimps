//
// Created by Zachary on 3/25/2022.
//
//TODO: make bundled vertix and edge structures
//TODO: make undirected graph with bundles and add edges and vertices
//TODO: Find away to move through graph and perform Breadth First Search, shortest path algo
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


struct GraphData { std::string Name; };
struct VertexProperty { std::string Name; };
struct EdgeProperty { std::string Name; };

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>;

int main(int argc, char** argv) {
//    cout << "Hello CS3353, your arguments were: ";
//    for (int i = 1; i < argc; i++) {
//        cout << argv[i];
//        if (i < argc - 1)
//            cout << ", ";
//    }
//    cout << endl << endl;
//
//    return 0;
    std::ifstream is;
    is.open("data/test_data_2.graphml");
    if(!is.is_open())
        std::cout << "hi" << std::endl;
    try {
        Graph graph;
        boost::dynamic_properties dp;

        boost::read_graphml(is, graph, dp);
        boost::print_graph(graph);
    }catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }


    // create a typedef for the Graph type
//    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
//
//    // Make convenient labels for the vertices
//    // defaults A=0, B=1, etc. so A-F are vertices and N is the number of vertices
//    enum { A, B, C, D, E, F, N };
//    const int num_vertices = N;
//    const char* name = "ABCDEF";
//
//    // writing out the edges in the graph
//    typedef std::pair<int, int> Edge;
//    Edge edge_array[] =
//            { Edge(A,B), Edge(A,C), Edge(B,C), Edge(D,C),
//              Edge(D,E), Edge(F,D), Edge(F,E) };
//    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);
//
//    // declare a graph object
//    Graph g(num_vertices);
//
//    // add the edges to the graph object
//    for (int i = 0; i < num_edges; ++i)
//        boost::add_edge(edge_array[i].first, edge_array[i].second, g);
//
//    auto vpair = boost::vertices(g);
//    for(auto iter = vpair.first; iter != vpair.second; iter++)
//        std::cout << "vertex " << *iter << std::endl;
//
//    auto epair = boost::edges(g);
//    for(auto iter = epair.first; iter != epair.second; iter++)
//        std::cout << "edge " << *iter << std::endl;



    // Breadth First Search


    return 0;
}