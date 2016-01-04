#include "../../helper/helper.1.h"

constexpr int MAX_NODES = 200;
std::array<char,MAX_NODES> color;
std::array<Vi,MAX_NODES> adjlist;

bool solve(int node, int col) {
    if (color[node]) {
        return color[node] == col;
    }
    color[node] = col;
    int new_col = -col;
    for (int neigh : adjlist[node]) {
        if (!solve(neigh,new_col))
            return false;
    }
    return true;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> edge;

    for(int ncase=0;;++ncase) {
        const int num_nodes = helper::string_to_int(reader());
        if (!num_nodes)
            break;
        if (ncase) {
            for (int i=0;i<num_nodes;++i) {
                color[i]=0;
            }
            for (int i=0;i<num_nodes;++i) {
                adjlist[i].clear();
            }
        }

        const int num_edges = helper::string_to_int(reader());
        for (int i=0;i<num_edges;++i) {
            edge = helper::num_array<int,2>(reader());
            adjlist[edge[0]].push_back(edge[1]);
            adjlist[edge[1]].push_back(edge[0]);
        }
        if (!solve(0,1)) {
            output.append("NOT ",4);
        }
        output.append("BICOLORABLE.\n");
    }
}

