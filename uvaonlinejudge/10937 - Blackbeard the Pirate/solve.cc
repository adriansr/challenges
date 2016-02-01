#include "../../helper/helper.1.h"
#include <cassert>
#include <bitset>
#include <limits>
#include <queue>
#include <utility>

using InputMap = std::vector<std::string>;

class Solver {
    public:
        static constexpr int IMPOSSIBLE = -1;
        static constexpr int INF = std::numeric_limits<int>::max();

        Solver(int H, int W, const InputMap& map)
            : height_(H), width_(W) {

            build_map(map);
        }

        int solve() {
            size_t num_nodes = poi_.size();
            // no treasures
            if (num_nodes < 2) {
                return num_nodes==1? 0 : IMPOSSIBLE; 
            }

            // no start position given
            if (poi_[0].x == -1) {
                return IMPOSSIBLE;
            }
            // check before hand if it's solvable (1)
            if (!all_nodes_reachable()) {
                return IMPOSSIBLE;
            }
            
            std::vector<Vi> distance;
            distance.resize(num_nodes);

            for (int node=0;node<num_nodes;++node) {
                distance[node] = bfs(node);
                for (int dist : distance[node]) {
                    if (dist == INF)
                        return IMPOSSIBLE;
                }
            }

            int cost = solve_search(distance);
            return cost != INF? cost : IMPOSSIBLE;
        }

    private:
        using Map = std::vector<std::vector<bool>>;

        struct Point {
            int x, y;
        };

        int solve_search(const std::vector<Vi>& distance) {
            struct State {
                int cost;
                int left_mask;
                int node;

                bool operator>(const State& other) const {
                    return cost>other.cost;
                }
            };
            const int num = distance.size();
            std::priority_queue<State,std::vector<State>,std::greater<State>> prioq;
            prioq.push({0,(1<<num)-1,0});
            int best = INF;
            do {
                State s = prioq.top();
                prioq.pop();
                if (s.cost >= best) {
                    break;
                }
                s.left_mask &= ~(1 << s.node);
                if (s.left_mask) {
                    for (int i=0;i<num;++i) {
                        if (s.left_mask & (1<<i)) {
                            prioq.push({s.cost + distance[s.node][i],s.left_mask,i});
                        }
                    }
                }
                else {
                    best = std::min(best,s.cost + distance[0][s.node]);
                }
            }
            while(!prioq.empty());
            return best;
        }

        Vi bfs(int poi_id) {
            const size_t num_poi = poi_.size();
            const int inf_value = INF;
            Vi distance(num_poi,inf_value);
            distance[poi_id] = 0;

            std::queue<std::pair<int,Point>> q;
            q.push(std::make_pair(0,poi_[poi_id]));
            
            Map visited(map_);
            
            auto push = [&q,&visited](int val, int x, int y) { 
                    if (!visited[y][x]) {
                        q.push(std::make_pair(val,Point({x,y})));
                    }
                };

            do {
                const auto current = q.front();
                q.pop();
                
                const auto& pos = current.second;
                if (!visited[pos.y][pos.x]) {
                    visited[current.second.y][pos.x] = true;
                    if (pos.y>0)
                        push(current.first+1,pos.x,pos.y-1);
                    if (pos.x>0)
                        push(current.first+1,pos.x-1,pos.y);
                    if (pos.y+1<height_)
                        push(current.first+1,pos.x,pos.y+1);
                    if (pos.x+1<width_)
                        push(current.first+1,pos.x+1,pos.y);

                    for (int i=0;i<num_poi;++i) {
                        if (i!=poi_id && poi_[i].x == pos.x && poi_[i].y == pos.y) {
                            if (current.first < distance[i]) {
                                distance[i] = current.first;
                            }
                        }
                    }
                }
            }
            while (!q.empty());
            return distance;
        }

        // builds bitmask map_ from input map
        void build_map(const InputMap& imap) {
            std::vector<bool> sample;
            sample.resize(width_);
            map_.resize(height_,sample);
            std::vector<Point> natives;
            // make room for start pos
            poi_.push_back({-1,-1});

            for (int y=0;y<height_;++y) {
                for (int x=0;x<width_;++x) {
                    bool blocked;
                    switch( imap[y][x] ) {
                        case '@':
                            poi_[0] = {x,y};
                            // no-break
                        case '.':
                            blocked = false;
                            break;
                        case '*':
                            natives.push_back( {x,y} );
                            // no-break
                        case '~':
                        case '#':
                            blocked = true;
                            break;
                        case '!':
                            poi_.push_back( {x,y} );
                            blocked = false;
                            break;
                        default:
                            assert(false);
                    }
                    map_[y][x] = blocked;
                }
            }
            
            // forbid native neighbours
            for (const Point pos : natives) {
                const int x = pos.x,
                          y = pos.y;
                if (y-1>=0) {
                    if (x-1>=0) map_[y-1][x-1] = true;
                    map_[y-1][x] = true;
                    if (x+1<width_) map_[y-1][x+1] = true;
                }

                if (x-1>=0) map_[y][x-1] = true;
                if (x+1<width_) map_[y][x+1] = true;

                if (y+1<height_) {
                    if (x-1>=0) map_[y+1][x-1] = true;
                    map_[y+1][x] = true;
                    if (x+1<width_) map_[y+1][x+1] = true;
                }
            }
        }

        bool all_nodes_reachable() const {
            for (const Point pos : poi_) {
                if (map_[pos.y][pos.x])
                    return false;
            }
            return true;
        }
        
        int height_,
            width_;
        Map map_;
        // points of interest
        // poi_[0] is start, rest are treasures
        std::vector<Point> poi_;
};

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    
    for (;;) {
        auto dims = helper::num_array<int,2>(reader());
        if (dims[0]==0)
            break;
        InputMap map;
        map.reserve(dims[0]);
        for (int y=0;y<dims[0];++y) {
            map.push_back(reader());
        }
        Solver solver(dims[0],dims[1],map);
        output.append(solver.solve()).endl();
    }
}

