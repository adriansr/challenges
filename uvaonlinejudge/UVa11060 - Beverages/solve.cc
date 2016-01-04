#include "../../helper/helper.1.h"
#include <queue>

constexpr int MAX_DRINKS = 101;
int num_drinks;
std::map<std::string,int> drink_to_id;
std::vector<std::string> id_to_drink;
std::array<std::bitset<MAX_DRINKS>,MAX_DRINKS> req_list;
std::array<int,MAX_DRINKS> req_count;
std::bitset<MAX_DRINKS> visited;
Vi solution;

void solve() {
    std::priority_queue<int,std::vector<int>,std::greater<int>> q;
    for (int node=1; node<=num_drinks; ++node) {
        if (!req_count[node])
            q.push(node);
    }

    do {
        int node = q.top();
        q.pop();
        solution.push_back(node);
        for (int other=1;other<=num_drinks;++other) {
            if (req_list[other][node]) {
                if (! --req_count[other]) {
                    q.push(other);
                }
            }
        }
    }
    while(!q.empty());
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    for (int ncase=0; reader.get_line(line); ++ncase) {

        if (ncase) {
            drink_to_id.clear();
            id_to_drink.clear();
            for (int i=1;i<=num_drinks;++i) {
                req_list[i].reset();
                req_count[i] = 0;
            }
            visited.reset();
            solution.clear();
        }

        num_drinks = helper::string_to_int(line);
        id_to_drink.push_back("<error>");
        for (int i=1;i<=num_drinks;++i) {
            reader.get_line(line);
            drink_to_id[line] = i;
            id_to_drink.push_back(line);
        }

        const int num_relations = helper::string_to_int(reader());

        for (int i=0;i<num_relations;++i) {
            reader.get_line(line);
            auto sep = line.find(' ');
            int weak_id   = drink_to_id[line.substr(0,sep)],
                strong_id = drink_to_id[line.substr(sep+1)];
            // GOTCHA: in judge there is a repeated relationship!
            if (!req_list[strong_id][weak_id]) {
                req_list[strong_id][weak_id] = true;
                req_count[strong_id]++;
            }
        }
        
        solve();

        output.append("Case #",6).append(ncase+1).append(": Dilbert should drink beverages in this order:");
        for (int id : solution) {
            output.append(' ').append( id_to_drink[id] );
        }
        output.append(".\n\n",3);
        reader();
    }
}

