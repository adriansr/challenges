#include "../../helper/helper.1.h"

constexpr int MAX_MONEY = 10201;

struct {
    int r[2][MAX_MONEY+2];
} dp;

int N,M,Mhardlimit;
Vii items;

bool money_hardlimit(int used) {
    return used <= Mhardlimit;
}

bool money_correct(int used) {
    return used <= M || (used>2000 && money_hardlimit(used));
}

int solve() {
    if (N == 0)
        return 0;
    Mhardlimit = M + 200;

    dp.r[0][0] = 1;
    dp.r[0][items[0].first] = 1 + items[0].second;

    int maxm = items[0].first;
    for (int num_item = 1; num_item < N; ++num_item) {
        int prev_row = (num_item+1)&1,
            row = num_item & 1;
        int limit = maxm + 1;
        for (int m=0;m<=limit;++m) {
            int mval = dp.r[prev_row][m]; 
            if (mval) {
#define MERGE(DEST,VAL) DEST = std::max(DEST,VAL)
                MERGE(dp.r[row][m],mval);
                int usedm = m+items[num_item].first;
                if (money_hardlimit(usedm)) {
                    MERGE(dp.r[row][usedm],mval + items[num_item].second);
                    if (usedm>maxm)
                        maxm = usedm;
                }
#undef MERGE
            }
        }
    }
    int maxval = 1,
        row = (N-1) & 1;
    for (int i = maxm; i >=0; --i) {
        int val = dp.r[row][i]; 
        if (val && money_correct(i) && val>maxval) {
            maxval = val;
        }
    }
    return maxval - 1;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> arr;
    for (int test_num=0; reader.get_line(line) ; ++test_num) {
        arr = helper::num_array<int,2>(line);
        M = arr[0];
        N = arr[1];
        if (test_num) {
            items.clear();
            dp = {};
        }
        items.reserve(N);
        for (int i=0;i<N;++i) {
            arr = helper::num_array<int,2>(reader());
            items.emplace_back(std::make_pair(arr[0],arr[1]));
        }
        int total_points = solve();
        output.append(total_points).endl();
    }
}

