#include "../../helper/helper.1.h"

int N,D,R;
Vi morning,
   afternoon;

int solve() {
    std::sort(morning.begin(),morning.end());
    std::sort(afternoon.rbegin(),afternoon.rend());

    int total_overtime = 0;

    for (int i = 0; i < N; ++i) {
        int sum = morning[i] + afternoon[i];
        int excess = sum - D;
        if (excess>0) {
            total_overtime += R * excess;
        }
    }
    return total_overtime;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    
    std::array<int,3> params;
    for (int case_num=0;;++case_num) {

        params = helper::num_array<int,3>(reader());
        if (params[0]==0)
            break;
        
        N = params[0];
        D = params[1];
        R = params[2];
        
        morning.reserve(N);
        afternoon.reserve(N);
        morning = helper::num_vector<int>(reader());
        afternoon = helper::num_vector<int>(reader());
        output.append(solve()).endl();
    }

}

