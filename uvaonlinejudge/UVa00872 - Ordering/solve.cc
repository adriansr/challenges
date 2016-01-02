#include "../../helper/helper.1.h"
#include <cassert>

constexpr int MIN_VARS = 2,
              MAX_VARS = 20,
              MAX_CONSTRAINTS = 50,
              MAX_ORDERINGS = 300;

std::array<Vi,MAX_VARS+1> adj_list;
std::bitset<MAX_VARS+1> secondary;
std::string nodes;

std::vector<std::string> solutions;
std::string current;

void cleanup() {
    for (auto& list : adj_list) {
        list.clear();
    }
    secondary.reset();
    nodes.clear();
    solutions.clear();
}

void toposort_do(int node,int visited) {
    if (! (visited&(1<<node)) ) {
        current.push_back(nodes[node]);
        if (current.size() < nodes.size()) {
            int new_visited = visited | (1<<node);
            for (int neigh : adj_list[node]) {
                toposort_do(neigh,new_visited);
            }
        }
        else {
            solutions.push_back(current);
        }
        current.pop_back();
    }
}

void toposort(int base_node) {
    current.clear();
    std::cerr << "toposort " << base_node << "\n";
    toposort_do(base_node,0);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<8192> reader;
    std::string line;

    int NUM_CASES = helper::string_to_int(reader());

    Vi nodelist;
    nodes.reserve(MAX_VARS);

    for (int ncase=0;ncase<NUM_CASES;++ncase) {
        
        reader();

        if (ncase) {
            cleanup();
        }
        
        reader.get_line(line);
        for (int i=0;i<line.size();i+=2) {
            nodes.push_back(line[i]);
        }
        
        reader.get_line(line);
        for (int i=0;i<line.size();i+=4) {
            size_t src = nodes.find(line[i]),
                   dst = nodes.find(line[i+2]);
            assert( src != nodes.npos );
            assert( dst != nodes.npos );
            std::cerr << "node " << line[i] << " to " << line[i+2] << "\n";
            secondary[dst] = true;
            adj_list[src].push_back(dst);
        }

        for (int i=0;i<nodes.size();++i) {
            if (adj_list[i].empty()) {
                for (int j=0;i<nodes.size();++i) {
                    if (j!=i) {
                        adj_list[i].push_back(j);
                        adj_list[j].push_back(i);
                    }
                }
            }
        }

        for (int i=0;i<nodes.size();++i) {
            if (!secondary[i]) {
                toposort(i);
            }
        }

        for (const auto& sol : solutions) {
            output.append(sol[0]);
            for (int i=1;i<nodes.size();++i) {
                output.append(' ').append(sol[i]);
            }
            output.endl();
        }
    }
}

