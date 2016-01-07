#include "../../helper/helper.1.h"
#include <queue>

constexpr int INF_COST = 1000000000;

int solve(int source_city,
          int destination_city,
          int max_capacity,
          int num_cities,
          const std::vector<Vii>& adj_list,
          const Vi& prices) {

    struct State {
        State(int city, int fuel, int cost) 
            : city(city), fuel_remaining(fuel), cost(cost) {}

        int city,
            fuel_remaining,
            cost;

        bool operator>(const State& other) const {
            return cost>other.cost || (cost==other.cost && city>other.city)
                 || (cost==other.cost && city==other.city && fuel_remaining>other.fuel_remaining);
        }
    };

    Vi cost;
    cost.resize(num_cities*(1+max_capacity),INF_COST);
    if (destination_city == source_city)
        return 0;
    
    auto cost_at = [&cost,&max_capacity](int city, int fuel) -> int& {
        return cost[ city * (1+max_capacity) + fuel];
    };

    std::priority_queue<State,std::vector<State>,std::greater<State>> prioq;
    prioq.emplace(source_city,0,0);

    do {
        const auto current = prioq.top();
        prioq.pop();
        
        // ignore unperfect solutions
        if (current.city == destination_city && current.fuel_remaining>0)
            continue;

        int &cost_ref = cost_at(current.city,current.fuel_remaining);
        if (cost_ref > current.cost) {
            cost_ref = current.cost;
            // don't expand past target city
            if (current.city == destination_city)
                continue;

            for (const auto& neighbour : adj_list[current.city]) {
                int remain = current.fuel_remaining - neighbour.second;
                if (remain>=0 &&  current.cost < cost_at(neighbour.first,remain)) {
                        prioq.emplace(neighbour.first,remain,current.cost);
                }
            }

            if (current.fuel_remaining < max_capacity) {
                prioq.emplace(current.city,
                              current.fuel_remaining+1,
                              current.cost + prices[current.city]);
            }
        }
    }
    while (!prioq.empty());

    // best cost is the one that results in reaching dest with empty tank
    return cost_at(destination_city,0);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    
    std::array<int,2> params = helper::num_array<int,2>(reader());
    const int num_cities = params[0],
              num_roads = params[1];

    Vi prices = helper::num_vector<int>(reader());
    std::vector<Vii> adj_list;
    adj_list.resize(num_cities);

    for (int i=0;i<num_roads;++i) {
        std::array<int,3> triplet = helper::num_array<int,3>(reader());
        adj_list[triplet[0]].emplace_back(triplet[1],triplet[2]);
        adj_list[triplet[1]].emplace_back(triplet[0],triplet[2]);
    }
    
    const int num_queries = helper::string_to_int( reader() );
    for (int i=0;i<num_queries;++i) {
        std::array<int,3> triplet = helper::num_array<int,3>(reader());
        const int capacity = triplet[0],
                  src = triplet[1],
                  dst = triplet[2];

        int price = solve(src,dst,capacity,num_cities,adj_list,prices);
        if (price!=INF_COST) {
            output.append(price).endl();
        }
        else {
            output.append("impossible\n");
        }
    }


}

