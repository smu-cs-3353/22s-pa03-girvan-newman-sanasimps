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

void Graph_helper::girvan_newman() {
    numNodes = (int)boost::num_vertices(graph);
    numEdges = (int)boost::num_edges(graph);


    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {
        graph[*iter].origDegree = (int)boost::degree(*iter, graph);
    }

    double mod = 0;
    while(true) {
        girvan_newman_helper();
        mod = get_modularity();
        if(mod > best_mod) {
            best_mod = mod;
            num_communities = boost::connected_components(graph, boost::make_assoc_property_map(max_comp));
        }

        if(boost::num_edges(graph) == 0)
            break;
    }

    std::vector<std::string> report(num_communities);

    std::cout << "Best Modularity: " << best_mod << std::endl;
    for(auto& c : max_comp) {
        std::cout << c.first << " in community " << c.second << std::endl;
        report[c.second] += std::to_string(c.first);
        report[c.second] += ", ";
    }
    print_report(report);



}

void Graph_helper::girvan_newman_helper() {


    // While Girvan-Neumann modularity is not satisfied

    // keep track of original vertex degree
    //      make 2 vertex properties: orig degree, new degree after edge is removed
    // loop girvan newman until new community is formed
    // for each node in community:
        //  add new degrees -> no of edges within communities
        //  add old degrees -> no of random edges
    // calculate modularity for each community

    // keep track of modularity and compare against current max
    // keep doing it until no more edges, or until it decreases (idk which is better)


    std::vector<int> component(numNodes);
    size_t num_components = boost::connected_components(graph, &component[0]);
    size_t new_num = num_components;
    std::cout << num_components << std::endl;

    while(new_num <= num_components) {
        //Breadth First Search to find shortest paths
        auto vpair = boost::vertices(graph);
        for(vertexIt iter = vpair.first; iter != vpair.second; iter++) {

            std::cout << "source: " << *iter << std::endl;
            std::cout << "visited: ";

            std::map<vd, vd> prev;

            breadth_first_search(prev, iter);
            reconstruct_paths(prev, iter);
            reset_tracking_data(iter);  // Resets tracking data for path construction
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

        new_num = boost::connected_components(graph, &component[0]);
        std::cout << "new comp num: " << new_num << std::endl;

    }

    // find communities

}

double Graph_helper::get_modularity () {
    set_degree();
//    std::vector<int> components(numNodes);
//    size_t num_comp = boost::connected_components(graph, &components[0]);

    std::map<vd, int> components;
    int num_components = boost::connected_components(graph, boost::make_assoc_property_map(components));

    std::vector<component> comp_mod(num_components);
    for(auto& p : components) {
        std::cout << "Vertex: " << p.first << " is in component " << p.second << std::endl;
        comp_mod[p.second].compEdges += graph[p.first].newDegree;
        comp_mod[p.second].totalEdges += graph[p.first].origDegree;
    }

    double mod = 0;
    for(auto& c : comp_mod) {
        double temp = c.compEdges - ((double)c.totalEdges * c.totalEdges) / (2 * numEdges);
        temp /= (2*numEdges);
        std::cout << temp << std::endl;
        mod += temp;
    }
    std::cout << "mod: " << mod << std::endl;
    return mod;


}

void Graph_helper::set_degree() {
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {
        graph[*iter].newDegree = (int)boost::degree(*iter, graph);
    }
}

void Graph_helper::breadth_first_search(std::map<vd, vd>& prev, vertexIt iter) {
    std::queue<vd> q;
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
}

void Graph_helper::reconstruct_paths (std::map<vd, vd>& prev, vertexIt iter) {
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
}

void Graph_helper::reset_tracking_data(vertexIt iter) {
    graph[*iter].foundPaths = true;
    auto vpair = boost::vertices(graph);
    for(auto v = vpair.first; v != vpair.second; v++)
        graph[*v].used = false;
}

void Graph_helper::print_report(std::vector<std::string> report) {
    std::ofstream out("data/communities.txt");
    if(out.is_open()) {
        out << "Community Report\n";
        for(int i = 0; i < report.size(); i++) {
            out << "community #" << i << ": [";
            out << report[i].substr(0, report[i].size()-2) << "]\n";
        }
    }
    out << "Modularity: " << best_mod;
}