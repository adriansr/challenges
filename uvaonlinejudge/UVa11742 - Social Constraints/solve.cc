#include "../../helper/helper.1.h"

#include <algorithm>
#include <cstring>

int N,M;

constexpr int MAX_N = 9;
const int factorial[MAX_N] = {0, 1, 2, 6, 24, 96, 576, 4032, 32256};

using Constraint = std::array<int,3>;
std::vector<Constraint> cv;
Vi state;

bool check_constraints() {
    int distance[N][N];
    // not really needed 
    memset(distance,0,sizeof(distance));

    for (int i = 0; i < N-1; ++i) {
        int person_i = state[i];
        for (int j = i+1; j < N; ++j) {
            int person_j = state[j];
            int a,b;
            if (person_i < person_j) {
                a = person_i;
                b = person_j;
            }
            else {
                a = person_j;
                b = person_i;
            }
            distance[a][b] = j - i;
        }
    }

    for (const auto& c : cv) {
        const int A = c[0],
                  B = c[1],
                  C = c[2];
        
        // not actually needed according to problem desc, A < B always
        int a, b;
        if (A < B) {
            a = A;
            b = B;
        }
        else {
            a = B;
            b = A;
        }
        int dist = distance[a][b];
        if (C>0 && dist> C) return false;
        if (C<0 && dist<-C) return false;
    }
    return true;
}

int solve() {
    state.resize(N);
    for (int i = 0; i < N; ++i)
        state[i] = i;

    int count = 0;

    while (std::next_permutation(state.begin(), state.end())) {
        if (check_constraints()) {
            ++ count;
        }
    }
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
            output.append(solve()).endl();
        }
        else {
            output.append(factorial[N]).endl();
        }
    }
}

