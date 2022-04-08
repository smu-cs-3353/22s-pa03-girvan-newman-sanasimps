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
    typedef boost::graph_traits<Graph>::vertex_descriptor vd;

    // While Girvan-Neumann modularity is not satisfied

    //Breadth First Search to find shortest paths
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {

        std::cout << "source: " << *iter << std::endl;
        std::cout << "visited: ";

        std::queue<vd> q;
        std::map<vd, vd> prev;
        int count = -1;

        q.push(*iter);
        graph[*iter].used = true;
        // source not included, when reconstructing use prev.count(source) != 0 as condition
        while(!q.empty()) {
            vd temp = q.front();
            q.pop();
            count++;
            auto neighbors = boost::adjacent_vertices(temp, graph);
            for(auto it = neighbors.first; it != neighbors.second; it++) {
                if(!graph[*it].used && !graph[*it].foundPaths) {
                    graph[*it].used = true;
                    q.push(*it);
                    graph[*it].distance = graph[temp].distance + 1;
                    prev.insert(std::pair<vd, vd>(*it, temp));
                    std::cout << *it << "(" << temp << ", " << graph[*it].distance << ") ";
                }
            }
        }

        std::cout << std::endl;
        // reconstruct paths
        for(auto& c : prev) {
            std::vector<vd> path;
            std::cout << c.first << ", " << c.second << ": ";
            if(!graph[c.first].used) {
                std::cout << "no path" << std::endl;
            } else {
                for(auto v = c.first; prev.count(v) != 0; v = prev[v]) {
                    path.push_back(v);
                }
                path.push_back(*iter);
                std::reverse(path.begin(), path.end());
                std::cout << "Path: ";
                int v = 0;
                std::cout << path[v] << " ";
                for(v = 1; v < path.size(); v++) {
                    std::cout << path[v] << " ";
                    auto e = boost::edge(path[v-1], path[v], graph);
                    graph[e.first].count++;
                }

                std::cout << std::endl;
            }
        }

        std::cout << std::endl;
        // Resets tracking data for path construction
        graph[*iter].foundPaths = true;
        auto t = boost::vertices(graph);
        for(auto te = t.first; te != t.second; te++)
           graph[*te].used = false;
    }

    // Finds the Highest Value
    auto e = boost::edges(graph);
    auto max = *(e.first);
    int maxCount = -1;
    for(auto ed = e.first; ed != e.second; ed++) {
        if(graph[*ed].count > maxCount) {
            max = *ed;
            maxCount = graph[*ed].count;

        }
        graph[*ed].count = 0;
    }
    std::cout << "max edge: " << max.m_source << " to " << max.m_target << std::endl;
    boost::remove_edge(max.m_source, max.m_target, graph);
    std::cout << "edge removed" << std::endl;
        // remove edge with highest value

    // find communities


}
