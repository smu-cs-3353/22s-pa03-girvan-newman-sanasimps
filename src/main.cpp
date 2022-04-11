//
// Created by Zachary on 3/25/2022.
//
#include "Graph_helper.h"

int main(int argc, char** argv) {

    if(argc == 2) {
        Graph_helper g;
        g.read_graphml(argv[1]);
        g.girvan_newman();
    } else {
        std::cout << "Please enter the correct amount of program arguments (2)" << std::endl;
    }

    return 0;
}