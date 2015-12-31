#include "../../helper/helper.1.h"
#include <queue>

constexpr int MAX_NODES = 501;

int num_nodes;

struct Node {
    Node() {}
    Node(int node, int seconds) : node(node), seconds(seconds) {}

    int node;
    int seconds;

    bool operator<(const Node& other) const {
        return seconds < other.seconds;
    }
    bool operator==(const Node& other) const {
        return seconds == other.seconds;
    }
    bool operator>(const Node& other) const {
        return seconds > other.seconds;
    }
};

std::array<std::vector<Node>,MAX_NODES> adj_list;
std::array<int,MAX_NODES> node_time;

struct Row {
    Row(int a,int b,int s)
        : node_a(a), node_b(b), seconds(s) {}

    int node_a,
        node_b,
        seconds;
};

std::vector<Row> vertices;

std::bitset<MAX_NODES> visited;
int num_visited;

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> params;
    std::array<int,3> row;
    std::vector<Node> empty;
    for (int ncase=0; reader.get_line(line) ; ++ncase) {
        params = helper::num_array<int,2>(line);

        num_nodes = params[0];
        //num_edges = params[1];
        if (!num_nodes)
            break;

        if (ncase) {
            adj_list.fill(empty);
            visited.reset();
            num_visited = 0;
            vertices.clear();
        }
        vertices.reserve(params[1]);
        for (int i=0; i<params[1]; ++i) {
            row = helper::num_array<int,3>(reader());
            vertices.emplace_back(row[0],row[1],row[2]);
            adj_list[row[0]].emplace_back(row[1],row[2]);
            adj_list[row[1]].emplace_back(row[0],row[2]);
        }

        std::priority_queue<Node,std::vector<Node>,std::greater<Node>> queue;
        queue.emplace(1,0);
        
        Node best_node(0,-1);

        do {
            const auto current = queue.top();
            queue.pop();

            if (visited[current.node]) {
                continue;
            }

            node_time[current.node] = current.seconds;
            if (current > best_node) {
                best_node = current;
            }

            visited[current.node] = true;
            ++num_visited;
            
            for (const auto adj : adj_list[current.node]) {
                queue.emplace(adj.node,current.seconds + adj.seconds);
            }
        }
        while (!queue.empty() && num_visited<num_nodes);

        int best_row_time10 = -1;
        int node_a,
            node_b;

        for (const auto v : vertices ) {
            int time_a = node_time[v.node_a],
                time_b = node_time[v.node_b];
            if (time_b < time_a) {
                std::swap(time_a,time_b);
            }
            if (time_b - time_a < v.seconds) {
                int meet = (time_b + time_a + v.seconds)*10/2;
                if (meet > best_row_time10) {
                    node_a = v.node_a;
                    node_b = v.node_b;
                    best_row_time10 = meet;
                }
            }
        }

        output.append("System #").append(ncase+1).endl()
              .append("The last domino falls after ");
        
        if (best_row_time10 > 10*best_node.seconds) {
            output.append(best_row_time10/10)
                  .append('.').append(best_row_time10%10)
                  .append(" seconds, between key dominoes ")
                  .append(node_a)
                  .append(" and ")
                  .append(node_b)
                  .append(".\n\n");
        }
        else {
            output.append(best_node.seconds).append(".0 seconds, at key domino ")
                 .append(best_node.node).append(".\n\n");
        }
    }   

}

