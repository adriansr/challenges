#include "../../helper/helper.1.h"

constexpr int MOD = 1000000;

int memo[101/*N*/][101/*K*/];

int solve(int N, int K) {
    if (K==1)
        return 1;
    
    int &saved = memo[N][K];
    if (saved)
        return saved;

    int count = 0;
    --K;
    for (int n = N; n>=0; --n) {
        count += solve(n,K);
    }

    count %= MOD;

    saved = count;
    return count;
}

int main() {
    helper::LineReader<> reader;
    std::string line;
    
    helper::BufferedStdout output;

    std::array<int,2> arr;
    for (int test_num = 0; reader.get_line(line) ; ++test_num) {
        
        arr = helper::num_array<int,2>(line);
        if (arr[0] == 0 && arr[1] == 0)
            break;

        const int N = arr[0],
                  K = arr[1];
        int result = K>0? solve(N,K) : 0;

        output.append(result).endl();
    }
}

