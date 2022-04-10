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

    girvan_newman_helper();

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
    get_modularity();

    // find communities

}

// vertex<custom> <- size of num component,
//  custom has e and a values
// itererate through component map and add e and a to component variable
// do all divisions and calculations at the end

void Graph_helper::get_modularity () {
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






/* def buildG(G, file_, delimiter_):
    #construct the weighted version of the contact graph from cgraph.dat file
    #reader = csv.reader(open("/home/kazem/Data/UCI/karate.txt"), delimiter=" ")
    reader = csv.reader(open(file_), delimiter=delimiter_)
    for line in reader:
        if len(line) > 2:
            if float(line[2]) != 0.0:
                #line format: u,v,w
                G.add_edge(int(line[0]),int(line[1]),weight=float(line[2]))
        else:
            #line format: u,v
            G.add_edge(int(line[0]),int(line[1]),weight=1.0)


# This method keeps removing edges from Graph until one of the connected components of Graph splits into two
# compute the edge betweenness
def CmtyGirvanNewmanStep(G):
    if _DEBUG_:
        print("Running CmtyGirvanNewmanStep method ...")
    init_ncomp = nx.number_connected_components(G)    #no of components
    ncomp = init_ncomp
    while ncomp <= init_ncomp:
        bw = nx.edge_betweenness_centrality(G, weight='weight')    #edge betweenness for G
        #find the edge with max centrality
        max_ = max(bw.values())
        #find the edge with the highest centrality and remove all of them if there is more than one!
        for k, v in bw.items():
            if float(v) == max_:
                G.remove_edge(k[0],k[1])    #remove the central edge
        ncomp = nx.number_connected_components(G)    #recalculate the no of components


# This method compute the modularity of current split
def _GirvanNewmanGetModularity(G, deg_, m_):
    New_A = nx.adj_matrix(G)
    New_deg = {}
    New_deg = UpdateDeg(New_A, G.nodes())
    #Let's compute the Q
    comps = nx.connected_components(G)    #list of components
    print('No of communities in decomposed G: {}'.format(nx.number_connected_components(G)))
    Mod = 0    #Modularity of a given partitionning
    for c in comps:
        EWC = 0    #no of edges within a community
        RE = 0    #no of random edges
        for u in c:
            EWC += New_deg[u]
            RE += deg_[u]        #count the probability of a random edge
        Mod += ( float(EWC) - float(RE*RE)/float(2*m_) )
    Mod = Mod/float(2*m_)
    if _DEBUG_:
        print("Modularity: {}".format(Mod))
    return Mod


def UpdateDeg(A, nodes):
    deg_dict = {}
    n = len(nodes)  #len(A) ---> some ppl get issues when trying len() on sparse matrixes!
    B = A.sum(axis = 1)
    i = 0
    for node_id in list(nodes):
        deg_dict[node_id] = B[i, 0]
        i += 1
    return deg_dict


# This method runs GirvanNewman algorithm and find the best community split by maximizing modularity measure
def runGirvanNewman(G, Orig_deg, m_):
    #let's find the best split of the graph
    BestQ = 0.0
    Q = 0.0
    while True:
        CmtyGirvanNewmanStep(G)
        Q = _GirvanNewmanGetModularity(G, Orig_deg, m_);
        print("Modularity of decomposed G: {}".format(Q))
        if Q > BestQ:
            BestQ = Q
            Bestcomps = list(nx.connected_components(G))    #Best Split
            print("Identified components: {}".format(Bestcomps))
        if G.number_of_edges() == 0:
            break
    if BestQ > 0.0:
        print("Max modularity found (Q): {} and number of communities: {}".format(BestQ, len(Bestcomps)))
        print("Graph communities: {}".format(Bestcomps))
    else:
        print("Max modularity (Q):", BestQ)


def main(argv):
    if len(argv) < 2:
        sys.stderr.write("Usage: %s <input graph>\n" % (argv[0],))
        return 1
    graph_fn = argv[1]
    G = nx.Graph()  #let's create the graph first
    buildG(G, graph_fn, ',')

    if _DEBUG_:
        print('G nodes: {} & G no of nodes: {}'.format(G.nodes(), G.number_of_nodes()))

    n = G.number_of_nodes()    #|V|
    A = nx.adj_matrix(G)    #adjacenct matrix

    m_ = 0.0    #the weighted version for number of edges
    for i in range(0,n):
        for j in range(0,n):
            m_ += A[i,j]
    m_ = m_/2.0
    if _DEBUG_:
        print("m: {}".format(m_))

    #calculate the weighted degree for each node
    Orig_deg = {}
    Orig_deg = UpdateDeg(A, G.nodes())

    #run Newman alg
    runGirvanNewman(G, Orig_deg, m_)

if __name__ == "__main__":
    sys.exit(main(sys.argv)) */