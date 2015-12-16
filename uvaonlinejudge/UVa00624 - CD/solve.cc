#include "../../helper/helper.1.h"

#include <algorithm>
#include <utility>

std::pair<int,int> solve(const std::vector<int> problem,
                         int base = 0, int partial_sum = 0, int mask = 0) {

    int best_sum = partial_sum;
    int best_mask = mask;

    for (int i = base; i < problem[1]; ++i) {
        int sum = problem[2+i] + partial_sum;
        if (sum <= problem[0]){
            auto r = solve(problem,i+1,sum,mask | (1<<i));
            if (r.first > best_sum) {
                best_sum = r.first;
                best_mask = r.second;
            }
        }
    }

    return std::make_pair(best_sum,best_mask);
}

int main() {
    
    helper::LineReader<> reader;
    std::string line;
    helper::BufferedStdout output;
    
    for(int test_count = 0; reader.get_line(line) ; ++test_count ) {
        
        const auto problem = helper::num_vector<int>(line);
        auto result = solve(problem);

        for (int i = 0; i < problem[1]; ++i) {
            if (result.second & (1<<i)) {
                output.append( problem[2+i] ).append(' ');
            }
        }
        output.append("sum:",4).append(result.first).endl();
    }
}

