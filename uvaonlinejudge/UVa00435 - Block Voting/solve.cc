#include "../../helper/helper.1.h"
#include <cstring>
#include <cstdio>

int majority;
int nparties;
Vi votes;
Vi power_index;

void for_each_party(int mask, std::function<void(int)> fn) {
    for (int party = 1; party <= nparties; ++party) {
        if (mask & (1<<party)) {
            fn(party);
        }
    }
}

void calc_power_index() {
    
    int limit = ( 1 << nparties);
    for (int mask1 = 0; mask1 < limit; ++mask1) {
        
        int sum = 0,
            mask = mask1 << 1;
        for_each_party(mask,[&sum](int party) { sum += votes[party]; });
        if ( sum < majority) {
            for_each_party(~mask,[sum](int party) { 
                    if ( sum + votes[party] >= majority) {
                        ++ power_index[party];
                    }
                });
        }
    }
}

void solve() {
    nparties = votes[0];

    power_index.clear();
    power_index.resize(nparties+1);
    
    int total = 0;
    for (int i = 1; i <= nparties; ++i) {
        total += votes[i];
    }
    majority = (total/2) + 1;

    calc_power_index();
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int N = helper::string_to_int(line);

    for (int test_num = 0; test_num < N; ++test_num) {
        
        reader.get_line(line);
        votes = helper::num_vector<int>(line);

        solve();

        for_each_party(-1,[&output](int party) { 
            output.append("party ",6)
                  .append(party)
                  .append(" has power index ")
                  .append(power_index[party])
                  .append('\n');
            });
        output.append('\n');
    }
}

