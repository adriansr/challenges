#include "../../helper/helper.1.h"

constexpr int MAX_NODES = 101;
std::array<Vi,MAX_NODES> adj_list;
std::bitset<MAX_NODES> has_prerequisite,
                       visited;
Vi solution;

void reset() {
    for (auto& list : adj_list) {
        list.clear();
    }
    solution.clear();
    has_prerequisite.reset();
    visited.reset();
}

void toposort(int node) {
    visited[node] = true;
    for (int neigh : adj_list[node]) {
        if ( ! visited[neigh] )
            toposort(neigh);
    }
    solution.push_back(node);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> params;
    for(int ncase=0; ; ++ncase) {
        
        params = helper::num_array<int,2>(reader());
        const int N = params[0],
                  M = params[1];
        
        if (!N)
            break;

        if (ncase) {
            reset();
        }

        for (int m=0;m<M;++m) {
            params = helper::num_array<int,2>(reader());
            adj_list[params[0]].push_back(params[1]);
            has_prerequisite[params[1]] = true;
        }
        
        for (int i=1;i<=N;++i) {
            if (!visited[i] && !has_prerequisite[i]) {
                toposort(i);
            }
        }
        
        output.append(solution[N-1]);
        for (int i=N-2;i>=0;--i) {
            output.append(' ').append(solution[i]);
        }
        output.endl();
    }
}

