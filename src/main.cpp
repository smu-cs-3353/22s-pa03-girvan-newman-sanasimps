//
// Created by Zachary on 3/25/2022.
//
#include "Graph_helper.h"
#include <chrono>

using namespace std;

int main(int argc, char** argv) {
    if(argc >= 2) {
        char* file = argv[1];
        Graph_helper g1;
        g1.read_graphml(file);
        std::cout << "Girvan Newman: " << std::endl;
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
        g1.girvan_newman();
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        chrono::duration<double> time = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        std::cout << "Time: " << time.count() << " seconds" << std::endl;

        std::cout << std::endl;

        Graph_helper g2;
        g2.read_graphml(file);
        std::cout << "Louvian: " << std::endl;
        t1 = chrono::high_resolution_clock::now();
        g2.louvain();
        t2 = chrono::high_resolution_clock::now();
        time = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        std::cout << "Time: " << time.count() << " seconds" << std::endl;
    } else {
        std::cout << "Please enter the correct amount of program arguments (2)" << std::endl;
    }

    return 0;
}