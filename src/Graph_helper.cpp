//
// Created by Daniel Ryan on 4/5/22.
//

#include "Graph_helper.h"


void Graph_helper::print_graph() {
    boost::print_graph(graph, get(&VertexProperty::name, graph));
}

void Graph_helper::print_edges() {
    auto epair = boost::edges(graph);
    for(auto iter = epair.first; iter != epair.second; iter++)
        std::cout << "edge " << *iter << std::endl;
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
        dp.property("name", boost::get(&VertexProperty::name, graph));
        dp.property("community", boost::get(&VertexProperty::community, graph));
        boost::read_graphml(is, graph, dp);
    }catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return;
    }
}
