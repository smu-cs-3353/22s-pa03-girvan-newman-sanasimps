//
// Created by Zachary on 3/25/2022.
//

#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <typeinfo>
#include <cxxabi.h>


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


    // create a typedef for the Graph type
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> Graph;

    // Make convenient labels for the vertices
    // defaults A=0, B=1, etc. so A-E are vertices and N is the number of vertices
    enum { A, B, C, D, E, N };
    const int num_vertices = N;
    const char* name = "ABCDE";

    // writing out the edges in the graph
    typedef std::pair<int, int> Edge;
    Edge edge_array[] =
            { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
              Edge(C,E), Edge(B,D), Edge(D,E) };
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

    // declare a graph object
    Graph g(num_vertices);

    // add the edges to the graph object
    for (int i = 0; i < num_edges; ++i)
        boost::add_edge(edge_array[i].first, edge_array[i].second, g);

    auto vpair = boost::vertices(g);
    for(auto iter = vpair.first; iter != vpair.second; iter++)
        std::cout << "vertex " << *iter << std::endl;

    auto epair = boost::edges(g);
    for(auto iter = epair.first; iter != epair.second; iter++)
        std::cout << "edge " << *iter << std::endl;

    return 0;
}