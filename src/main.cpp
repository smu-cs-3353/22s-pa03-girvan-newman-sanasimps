//
// Created by Zachary on 3/25/2022.
//
//TODO: Find away to move through graph and perform Breadth First Search, shortest path algo
#include "Graph_helper.h"

int main(int argc, char** argv) {

    Graph_helper g;
    g.read_graphml("data/test_data_1.graphml");
    //g.print_graph();
    g.Breadth_first_search();

    return 0;
}