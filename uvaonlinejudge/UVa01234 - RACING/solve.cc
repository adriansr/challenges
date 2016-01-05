#include "../../helper/helper.1.h"

#include <queue>

struct Edge {
    Edge(int vertex, int weight) : vertex(vertex), weight(weight) {}
    int vertex,weight;

    bool operator<(const Edge& other) const {
        return weight<other.weight || (weight==other.weight && vertex<other.vertex);
    }

    bool operator>(const Edge& other) const {
        return weight>other.weight || (weight==other.weight && vertex>other.vertex);
    }
};


using AdjacencyList = std::vector<std::vector<Edge>>;

int max_st_prim(int num_vertex,const AdjacencyList& adj_list) {
    
    std::priority_queue<Edge> prioq;

    std::vector<bool> visited;
    visited.resize(num_vertex+1);
    visited[1] = true;

    for (const auto edge : adj_list[1]) {
        prioq.push(edge);
    }

    int cost = 0;

    for (int pending=num_vertex-1;pending>0 && !prioq.empty();) {
        const Edge e = prioq.top();
        prioq.pop();
        
        if (!visited[e.vertex]) {
            visited[e.vertex] = true;
            cost += e.weight;
            /*std::cerr << "Added vertex to " << e.vertex << " weight " << e.weight
                      << " for a total of " << cost << '\n';*/
            --pending;
            for (const auto edge : adj_list[e.vertex]) {
                if (!visited[edge.vertex]) 
                    prioq.push(edge);
            }
        }
    }

    return cost;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    const int NUM_CASES = helper::string_to_int(reader());

    for (int num_case=0;num_case < NUM_CASES;++num_case) {
        
        std::array<int,2> problem = helper::num_array<int,2>(reader());
        const int num_vertex = problem[0],
                  num_edges  = problem[1];

        AdjacencyList adj_list;
        adj_list.resize(num_vertex+1);

        int full_cost = 0;
        for (int i=0;i<num_edges;++i) {
            std::array<int,3> edge = helper::num_array<int,3>(reader());
            adj_list[edge[0]].emplace_back( edge[1], edge[2] );
            adj_list[edge[1]].emplace_back( edge[0], edge[2] );
            full_cost += edge[2];
        }

        output.append( full_cost - max_st_prim(num_vertex,adj_list) ).endl();
    }
}

