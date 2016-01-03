#include "../../helper/helper.1.h"

constexpr int MIN_VARS = 2,
              MAX_VARS = 20,
              MAX_CONSTRAINTS = 50,
              MAX_ORDERINGS = 300;

std::array<Vi,MAX_VARS+1> req_list;
std::string nodes;

std::vector<std::string> solutions;
std::string current;

void cleanup() {
    for (auto& list : req_list) {
        list.clear();
    }
    nodes.clear();
    solutions.clear();
}

void do_solve(int node,int visited) {
    if (! (visited&(1<<node)) ) {
        for (int req : req_list[node]) {
            if (!(visited&(1<<req))) {
                return;
            }
        }

        visited |= (1<<node);
        current.push_back(nodes[node]);
        if (current.size() < nodes.size()) {
            for (int i=0;i<nodes.size();++i) {
                if (! (visited&(1<<i)) ) {
                    do_solve(i,visited);
                }
            }
        }
        else {
            solutions.push_back(current);
        }
        current.pop_back();
    }
}

void solve(int base_node) {
    current.clear();
    do_solve(base_node,0);
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
            output.endl();
        }
        
        reader.get_line(line);
        for (int i=0;i<line.size();i+=2) {
            nodes.push_back(line[i]);
        }
        std::sort(nodes.begin(),nodes.end());
        
        reader.get_line(line);
        for (int i=0;i<line.size();i+=4) {
            size_t src = nodes.find(line[i]),
                   dst = nodes.find(line[i+2]);
            req_list[dst].push_back(src);
        }

        for (int i=0;i<nodes.size();++i) {
            if (req_list[i].empty()) {
                solve(i);
            }
        }
        
        if (solutions.size()) {
            for (const auto& sol : solutions) {
                output.append(sol[0]);
                for (int i=1;i<nodes.size();++i) {
                    output.append(' ').append(sol[i]);
                }
                output.endl();
            }
        }
        else {
            output.append("NO\n");
        }
    }
}

