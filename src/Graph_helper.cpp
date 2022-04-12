//
// Created by Daniel Ryan on 4/5/22.
//

#include "Graph_helper.h"

void Graph_helper::print_graph() {
    boost::print_graph(graph);
}

void Graph_helper::print_edges() {
    // returns iterator pairs where the first value is the front of the list of edge descriptors
    //  and the second is past the end
    auto epair = boost::edges(graph);
    for(auto iter = epair.first; iter != epair.second; iter++) {
        std::cout << "edge " << graph[*iter].Name << std::endl;
    }
}

void Graph_helper::print_vertices() {
    // returns iterator pairs where the first value is the front of the list of vertex descriptors
    //  and the second is past the end
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++)
        std::cout << "vertex " << *iter << std::endl;
}

void Graph_helper::read_graphml(const char* file) {
    // use istream to open file
    std::ifstream inf;
    inf.open(file);
    if(!inf.is_open()) {
        std::cout << "[ERROR]: could not open Graphml file" << std::endl;
        return;
    }

    // try-catch to catch any errors reading Graphml file
    try {
        // to read the community property of each node
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("value", boost::get(&VertexProperty::community, graph));
        boost::read_graphml(inf, graph, dp);
    }catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return;
    }

}

void Graph_helper::girvan_newman() {
    // keeps track of total initial edges and nodes for later
    numNodes = (int)boost::num_vertices(graph);
    numEdges = (int)boost::num_edges(graph);

    // keeps track of the original degree of each node for modularity calculation
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {
        graph[*iter].origDegree = (int)boost::degree(*iter, graph);
    }

    std::cout << "mod trends: ";
    double mod = 0;
    while(true) {
        girvan_newman_helper(); // to get edge betweeness centrality and remove edges
        mod = get_modularity(); // to get new mod when num of components increase
        if(mod > best_mod) {
            best_mod = mod;
            num_communities = boost::connected_components(graph, boost::make_assoc_property_map(max_comp));
            std::cout << "^";
        } else
            std::cout << ".";
        std::cout.flush();

        // stops removing edges once no more edges remain
        if(boost::num_edges(graph) == 0)
            break;
    }
    std::cout << std::endl << std::endl;

    // gets communities and prepares community report
    std::vector<std::string> report(num_communities);
    std::cout << "Best Modularity: " << best_mod << std::endl;
    for(auto& c : max_comp) {
//        std::cout << c.first << " in community " << c.second << std::endl;
        report[c.second] += std::to_string(c.first);
        report[c.second] += "-";
        report[c.second] += std::to_string(graph[c.first].community);
        report[c.second] += ", ";
    }
    print_report(report);

}

void Graph_helper::girvan_newman_helper() {

    // keeps track of beginning number of components
    std::vector<int> component(numNodes);
    size_t num_components = boost::connected_components(graph, &component[0]);
    size_t new_num = num_components;
    //std::cout << num_components << std::endl;

    // ends loop once number of components in the graph increase
    while(new_num <= num_components) {
        //Breadth First Search to find shortest paths
        auto vpair = boost::vertices(graph);
        for(vertexIt iter = vpair.first; iter != vpair.second; iter++) {

            //std::cout << "source: " << *iter << std::endl;
            //std::cout << "visited: ";

            std::map<vd, vd> prev;  // for path reconstructing

            breadth_first_search(prev, iter);
            reconstruct_paths(prev, iter);
            reset_tracking_data(iter);  // Resets tracking data for path construction
        }

        // finds the highest value for edge betweeness centrality
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

        // removes edge and recalculates number of components

        //std::cout << "max edge: " << max.m_source << " to " << max.m_target << std::endl;
        boost::remove_edge(max.m_source, max.m_target, graph);
        new_num = boost::connected_components(graph, &component[0]);
        //std::cout << "new comp num: " << new_num << std::endl;

    }
}

void Graph_helper::louvain() {
    // keeps track of total initial edges and nodes for later
    numNodes = (int)boost::num_vertices(graph);
    numEdges = (int)boost::num_edges(graph);

    Graph temp = graph;
    boost::remove_edges(graph);

    while(true) {
        //put each node of G in its own community.
        num_communities = boost::connected_components(graph, boost::make_assoc_property_map(max_comp));

        louvain_helper(temp);
        if (curr_mod > best_mod) {
            best_mod = curr_mod;
        }
        else
            break;
    }
}

void Graph_helper::louvain_helper(Graph& temp) {
    bool nodesMoved = false;
    do {
        auto vpair = boost::vertices(temp);
        for(vertexIt iter = vpair.first; iter != vpair.second; iter++) {
            if (join_nodes(temp, iter))
                nodesMoved = true;
        }
    } while(nodesMoved);
}

bool Graph_helper::join_nodes(Graph& temp, vertexIt iter) {
    bool result = false;
    auto neighbors = boost::adjacent_vertices(iter, temp);
    ed* e = nullptr;
    for (auto it = neighbors.first; it != neighbors.second; it++) {
        boost::add_edge(it, iter, graph);
        double mod = get_modularity();
        if (mod > curr_mod) {
            curr_mod = mod;
            if (e)
                boost::remove_edge(e->m_source, e->m_target, graph);
            e = &boost::edge(it, iter, graph);
            result = true;
        }
        else
            boost::remove_edge(it, iter, graph);
    }
    return result;
}

double Graph_helper::get_modularity () {
    set_degree();   // to set current degrees for each vertex

    // to get number of components and the component for each vertex using a map
    std::map<vd, int> components;
    int num_components = boost::connected_components(graph, boost::make_assoc_property_map(components));

    // counts total component edges and total random edges for each component in the graph
    std::vector<component> comp_mod(num_components);
    for(auto& p : components) {
        //std::cout << "Vertex: " << p.first << " is in component " << p.second << std::endl;
        comp_mod[p.second].compEdges += graph[p.first].newDegree;
        comp_mod[p.second].totalEdges += graph[p.first].origDegree;
    }

    // calculates modularity using Newman's modularity formula, can be found in README
    double mod = 0;
    for(auto& c : comp_mod) {
        double temp = c.compEdges - ((double)c.totalEdges * c.totalEdges) / (2 * numEdges);
        temp /= (2*numEdges);
        //std::cout << temp << std::endl;
        mod += temp;
    }
    return mod;


}

void Graph_helper::set_degree() {
    // updates degree of each vertex for current partition
    auto vpair = boost::vertices(graph);
    for(auto iter = vpair.first; iter != vpair.second; iter++) {
        graph[*iter].newDegree = (int)boost::degree(*iter, graph);
    }
}

void Graph_helper::breadth_first_search(std::map<vd, vd>& prev, vertexIt iter) {

    // normal breadth first search, but also keeps track of prev nodes for each visit for path reconstruction
    std::queue<vd> q;
    int count = -1;

    q.push(*iter);
    graph[*iter].used = true;
     //source not included, when reconstructing use prev.count(source) != 0 as condition
    while(!q.empty()) {
        vd temp = q.front();
        q.pop();
        count++;

        // iterates through adjacent vertices before moving to next level
        auto neighbors = boost::adjacent_vertices(temp, graph);
        for(auto it = neighbors.first; it != neighbors.second; it++) {
            if(!graph[*it].used) {
                graph[*it].used = true;
                q.push(*it);
                graph[*it].distance = graph[temp].distance + 1;
                prev.insert(std::pair<vd, vd>(*it, temp));
                //std::cout << *it << "(" << temp << ", " << graph[*it].distance << ") ";
            }
        }
    }
}

void Graph_helper::reconstruct_paths (std::map<vd, vd>& prev, vertexIt iter) {

    // uses prev list from BFS to reconstruct the shortest path from one node to every other node
    for(auto& c : prev) {
        std::vector<vd> path;
        //std::cout << c.first << ", " << c.second << ": ";
        if(!graph[c.first].used) {
            std::cout << "no path" << std::endl;
        } else {
            for(auto v = c.first; prev.count(v) != 0; v = prev[v]) {
                path.push_back(v);
            }
            path.push_back(*iter);
            std::reverse(path.begin(), path.end());
            //std::cout << "Path: ";
            int v = 0;
            //std::cout << path[v] << " ";
            for(v = 1; v < path.size(); v++) {
                //std::cout << path[v] << " ";
                auto e = boost::edge(path[v-1], path[v], graph);
                graph[e.first].count++;
            }
        }
    }
}

void Graph_helper::reset_tracking_data(vertexIt iter) {
    auto vpair = boost::vertices(graph);
    for(auto v = vpair.first; v != vpair.second; v++)
        graph[*v].used = false;
}

void Graph_helper::print_report(std::vector<std::string> report) const {
    // outputs file with each component and their vertices
    std::ofstream out("data/community_report.txt");
    if(out.is_open()) {
        out << "Community Report\n";
        std::cout << "Community Report" << std::endl;
        for(int i = 0; i < report.size(); i++) {
            out << "community #" << i << ": [";
            out << report[i].substr(0, report[i].size()-2) << "]\n";
            std::cout << "community #" << i << ": [";
            std::cout << report[i].substr(0, report[i].size()-2) << "]\n";
        }
    }
    out << "Modularity: " << best_mod;
}