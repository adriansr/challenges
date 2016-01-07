#include "../../helper/helper.1.h"

constexpr int MAX_NODES = 10001,
                    INF = 2000000000;

std::array<Vii,MAX_NODES> adj_list;
Vi dist;
int num_nodes;


void cleanup() {
    for (int i=0;i<num_nodes;++i) {
        adj_list[i].clear();
    }
    dist.clear();
}

struct UpdateDistances {
    static bool exec(int node, int new_dist) {
        dist[node] = new_dist;
        return false;
    }
};

struct TerminateOnUpdate {
    static bool exec(int,int) {
        return true;
    }
};

template<class F>
bool update() {
    for (int node=0;node<num_nodes;++node) {
        int dist_u = dist[node];
        if (dist_u != INF) {
            for (const auto& neigh : adj_list[node]) {
                int new_dist = dist_u + neigh.second;
                if (new_dist < dist[neigh.first]) {
                    if (F::exec(neigh.first,new_dist))
                        return true;
                }
            }
        }
    }
    return false;
}

bool solve() {
    dist.resize(num_nodes,INF);
    dist[0] = 0;

    for (int i=0,limit=num_nodes-1;i<limit;++i) {
        update<UpdateDistances>();
    }

    return update<TerminateOnUpdate>();
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
   
    const int NUM_CASES = helper::string_to_int(reader());

    std::array<int,2> params;
    std::array<int,3> edge;

    dist.reserve(MAX_NODES);
    for (int case_num=0;case_num<NUM_CASES;++case_num) {
        if (case_num) {
            cleanup();
        }
        
        params = helper::num_array<int,2>(reader());
        num_nodes = params[0];

        for (int i=0;i<params[1];++i) {
            edge = helper::num_array<int,3>(reader());
            adj_list[edge[0]].emplace_back(edge[1],edge[2]);
        }

        if (!solve()) {
            output.append("not ",4);
        }
        output.append("possible\n",9);
    }
}

