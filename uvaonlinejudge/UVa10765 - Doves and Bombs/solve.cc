#include "../../helper/helper.1.h"
#include <vector>
#include <algorithm>
#include <queue>

class StationBomber {
public:
	enum { UNVISITED = 0 };

	StationBomber(int num_stations) : num_stations_(num_stations) {
        adj_list_.resize(num_stations_);
        num_dfs_.resize(num_stations_,UNVISITED);
        low_dfs_.resize(num_stations_,UNVISITED);
        dfs_parent_.resize(num_stations_,UNVISITED);
        is_articulation_vertex_.resize(num_stations_);
        //counter_=UNVISITED;
    }

    void add_edge(int a, int b) {
        //std::cerr << "edge " << a << " " << b << "\n";
        adj_list_[a].push_back(b);
        adj_list_[b].push_back(a);
    }


    void print_bombable(int count) {
        
        solve();
        const auto articulations = is_articulation_vertex_;

        struct BombableStation {
            BombableStation(int id, int nn) : id(id), num_neighbours(nn) {}

            int id,
                num_neighbours;

            bool operator<(const BombableStation& other) const {
                return num_neighbours<other.num_neighbours 
                    || (num_neighbours==other.num_neighbours && id>other.id);
            }
        };

        std::priority_queue<BombableStation> prioq;
        for (int station = 0; station < num_stations_;++station) {
            prioq.emplace(station,articulations[station]?
                                    count_components_by_removal(station) : 1);
        }

        for (int i=0;i<count && !prioq.empty();++i) {
                const auto station = prioq.top();
                prioq.pop();
                std::cout << station.id << ' ' << station.num_neighbours << '\n';
        }
        std::cout << '\n';
    }

private:
    void solve() {
        for (int station=0;station<num_stations_;++station) {
            if (num_dfs_[station] == UNVISITED) {
                counter_=UNVISITED;
                dfs_root_ = station;
                root_children_ = 0;
                dfs(station);
                is_articulation_vertex_[station] = root_children_ > 1;
            }
        }
    }
    
    void dfs(int station) {

        num_dfs_[station] = low_dfs_[station] = ++counter_;

        for (int neighbour : adj_list_[station]) {
            if (num_dfs_[neighbour] == UNVISITED) {
                if (station == dfs_root_)
                    root_children_ ++;

                //std::cerr << "visit " << neighbour << "\n";
                dfs_parent_[neighbour] = station;
                dfs(neighbour);

                if (low_dfs_[neighbour]>=num_dfs_[station])
                    is_articulation_vertex_[station] = true;

                low_dfs_[station] =std::min(low_dfs_[station],low_dfs_[neighbour]);
            }
            else if (neighbour != dfs_parent_[station])
                   low_dfs_[station]=std::min(low_dfs_[station],num_dfs_[neighbour]);
        }
        //std::cerr << "dfs["<<station<<"]: low:"<<low_dfs_[station] 
        //                             << " num:" << num_dfs_[station] << "\n";
    }
    
    int count_components_by_removal(int target) {
        num_dfs_.assign(num_dfs_.size(),UNVISITED);
        int num_components = 0;
        num_dfs_[target] = 1;
        for (int station : adj_list_[target] ) {
            if (num_dfs_[station] == UNVISITED) {
                num_components++;
                counter_=UNVISITED;
                dfs(station);
            }
        }

        return num_components;
    }

    const int num_stations_;
    std::vector<std::vector<int>> adj_list_;
    std::vector<int> num_dfs_,
                     low_dfs_,
                     dfs_parent_;
    std::vector<bool> is_articulation_vertex_;
    int counter_,
        dfs_root_,
        root_children_;
};


int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> arr;
    while (reader.get_line(line)) {
        arr = helper::num_array<int,2>(line);

        const int num_stations = arr[0],
                  target_stations = arr[1];
        
        if (num_stations==0)
            break;

        StationBomber bomber(num_stations);
        
        while (reader.get_line(line)) {
            arr = helper::num_array<int,2>(line);
            if (arr[0]==-1)
                break;
            bomber.add_edge(arr[0],arr[1]);
        }

        bomber.print_bombable(target_stations);
    }
}

