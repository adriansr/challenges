#include "../../helper/helper.1.h"

#include <algorithm>
#include <cstring>

int N,M;

constexpr int MAX_N = 9;
const int factorial[MAX_N] = {0, 1, 2, 6, 24, 120, 720, 5040, 40320};

using Constraint = std::array<int,3>;
std::vector<Constraint> cv;
Vi state;

bool check_constraints() {
    int distance[N][N];

    for (int i = 0; i < N-1; ++i) {
        int vi = state[i];
        for (int j = i+1; j < N; ++j) {
            int vj = state[j];
            distance[vi][vj] = j - i;
            distance[vj][vi] = j - i;
        }
    }

    for (const auto& c : cv) {
        const int A = c[0],
                  B = c[1],
                  C = c[2];
        
        int dist = distance[A][B];
        if ((C>0 && dist> C) || (C<0 && dist<-C))
            return false;
    }
    return true;
}

int solve() {
    state.resize(N);
    for (int i = 0; i < N; ++i)
        state[i] = i;

    int count = 0;

    do
    {
        if (check_constraints()) {
            ++ count;
        }
    }
    while (std::next_permutation(state.begin(), state.end()));

    return count;
}

int main() {

    helper::LineReader<> reader;
    std::string line;
    helper::BufferedStdout output;

    std::array<int,2> params;

    for(int test_count = 0; reader.get_line(line) ; ++test_count ) {

        params = helper::num_array<int,2>(line);
        if (params[0] == 0)
            break;
        
        N = params[0];
        M = params[1];
        
        if (M) {
            if (test_count) {
                cv.clear();
                cv.reserve(M);
            }
            for (int i = 0; i < M; ++i) {
                reader.get_line(line);
                cv.push_back(helper::num_array<int,3>(line));
            }
            output.append(solve());
        }
        else {
            output.append(factorial[N]);
        }
        output.endl();
    }
}

