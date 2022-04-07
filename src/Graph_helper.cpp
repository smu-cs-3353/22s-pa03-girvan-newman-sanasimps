//
// Created by Daniel Ryan on 4/5/22.
//

#include "Graph_helper.h"


void Graph_helper::print_graph() {
    boost::print_graph(graph);
}

void Graph_helper::print_edges() {
    auto epair = boost::edges(graph);
    for(auto iter = epair.first; iter != epair.second; iter++) {
        graph[*iter].Name = "taco";
        std::cout << "edge " << graph[*iter].Name << std::endl;

    }
}

void Graph_helper::print_vertices() {
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++)
        std::cout << "vertex " << *iter << std::endl;
}

void Graph_helper::read_graphml(const char* file) {
    std::ifstream is;
    is.open(file);
    if(!is.is_open()) {
        std::cout << "[ERROR]: could not open Graphml file" << std::endl;
        return;
    }

    try {
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("community", boost::get(&VertexProperty::community, graph));
        boost::read_graphml(is, graph, dp);
    }catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return;
    }

}

void Graph_helper::Breadth_first_search () {

    // While Girvan-Neumann modularity is not satisfied
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {
        auto out_edges = boost::out_edges(*iter, graph);
        std::cout << *iter << " <--> ";
        for(auto it = out_edges.first; it != out_edges.second; it++)
            std::cout << boost::target(*it, graph) << " ";
        std::cout << std::endl;


    }
        // loop through each node
            // get arr of prev.
            // reconstruct path and add 1 to each edge

        // remove edge with highest value

    // find communities


}
