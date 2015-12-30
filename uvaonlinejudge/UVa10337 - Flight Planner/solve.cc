#include "../../helper/helper.1.h"

constexpr int MAX_ALTITUDE = 10;
constexpr int MAX_W = 100;
constexpr int MAX_COST = 1000000000;
constexpr int COST_CRUISE = 30,
              COST_CLIMB  = 60,
              COST_DESCENT = 20;

int W;
std::array< std::array<int,MAX_W> , MAX_ALTITUDE> wind_array,
                                                  memo;

int solve(int pos, int alt) {
    
    if (pos<W) {
        // prune when this path can't lead to a successful landing
        if (pos + alt > W) 
            return MAX_COST;

        int &saved = memo[alt][pos];
        if (saved) {
            return saved;
        }

        int best_cost = MAX_COST,
            wind = wind_array[alt][pos];

        best_cost = COST_CRUISE - wind + solve(pos+1,alt);

        if (alt+1 < MAX_ALTITUDE) {
            best_cost = std::min(best_cost,
                                 COST_CLIMB - wind + solve(pos+1,alt+1));
        }
        if (alt>0) {
            best_cost = std::min(best_cost,
                                 COST_DESCENT - wind + solve(pos+1,alt-1));
        }
        
        saved = best_cost;
        return best_cost;
    }
    return alt == 0? 0 : MAX_COST;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int N = helper::string_to_int( line );

    for (int ncase=0;ncase<N;++ncase) {
        while( reader.get_line(line) && line.empty() );

        const int X = helper::string_to_int( line );
        W = X / 100;
        Vi row;
        row.reserve(W);
        for (int i=MAX_ALTITUDE-1;i>=0;--i) {
            reader.get_line(line);
            row = helper::num_vector<int>( line );
            for (int pos=0;pos<W;++pos) {
                wind_array[i][pos] = row[pos];
                memo[i][pos] = 0;
            }
        }
        
        output.append( solve(0,0) ).endl().endl();
    }
}

