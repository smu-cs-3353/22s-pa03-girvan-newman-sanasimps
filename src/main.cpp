//
// Created by Zachary on 3/25/2022.
//
//TODO: make bundled vertix and edge structures
//TODO: make undirected graph with bundles and add edges and vertices
//TODO: Find away to move through graph and perform Breadth First Search, shortest path algo
#include "Graph_helper.h"

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

    Graph_helper g;
    g.read_graphml("data/test_data_2.graphml");
    g.print_graph();

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


    return 0;
}