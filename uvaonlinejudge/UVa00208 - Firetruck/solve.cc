#include "../../helper/helper.1.h"

constexpr int MAX_CORNERS = 21;

using Path = std::vector<int>;

helper::BufferedStdout output;

struct Problem {

    static std::array<Vi, MAX_CORNERS> adj_list;
    static int target_corner;

    static void reset() {
        for (int i = 0; i < MAX_CORNERS; ++i) {
            adj_list[i].clear();
        }
    }

    static void solve();
    static void solve_at(int pos, int visited);

};

std::array<Vi, MAX_CORNERS> Problem::adj_list;
int Problem::target_corner;
std::array<int,MAX_CORNERS+1> list;
int count_routes;

void Problem::solve_at(int pos, int visited) {
    int current = list[pos++];
    const Vi& adj = adj_list[current];
    visited |= 1 << current;

    //for ( int n : adj ) {
    for (int i = 0, len = adj.size(), n; i < len; ++i) {
        n = adj[i];
        if ( ! ( visited & (1<<n)) ) {
            list[pos] = n;
            if (n != target_corner) {
                solve_at(pos,visited);
            }
            else {
                ++count_routes;
                output.append('1');
                for (int i = 1; i <= pos; ++i) {
                    output.append(' ').append(list[i]);
                }
                output.endl();
            }
        }
    }
}

void Problem::solve() {
    list[0] = 1;
    count_routes = 0;
    solve_at(0,0);
}

int main() {
    helper::LineReader<> reader;
    std::string line;
    
    Problem p;
    std::array<int,2> corners;
    for (int test_num = 1; reader.get_line(line) ; ++test_num) {
        if (test_num > 1)
            p.reset();

        p.target_corner = helper::string_to_int(line);

        for(;;) {
            reader.get_line(line);
            corners = helper::num_array<int,2>(line);
            const int a = corners[0],
                      b = corners[1];
            if (a==0 && b==0) {
                break;
            }
            p.adj_list[a].push_back(b);
            p.adj_list[b].push_back(a);
        }
        //helper::backtrack(p);
        output.append("CASE ",5).append(test_num).append(":\n",2);
        p.solve();
        output.append("There are ").append(count_routes)
              .append(" routes from the firestation to streetcorner ")
              .append(p.target_corner).append(".\n");
        //output.flush();
    }
}

