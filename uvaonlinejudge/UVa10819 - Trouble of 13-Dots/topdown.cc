#include "../../helper/helper.1.h"

#define USE_MEMO 1

#if USE_MEMO
constexpr int MAX_MONEY = 10200,
              MAX_NUM   = 100;

struct {
    int memo[MAX_NUM+2][MAX_MONEY+2];
} dp;
#endif

int N,M;
Vii items;

bool money_hardlimit(int used) {
    return used <= M+200;
}

bool money_correct(int used) {
    return used <= M || (used>2000 && money_hardlimit(used));
}

int solve(int selected_item, int money_used) {
#if USE_MEMO
    int val = dp.memo[selected_item][money_used];
    if (val) {
        return val!=-1? val : 0;
    }
#endif
    
    int points = 0;
    if (selected_item < N) {
        const auto item = items[selected_item];
        int next_item = selected_item + 1;
        if (money_hardlimit(money_used + item.first)) {
            int next_buying = solve(next_item,money_used+item.first);
            points = solve(next_item,money_used);
            if (next_buying != -10)
                points = std::max( points,
                                   item.second + next_buying );
        }
        else {
            points = solve(next_item,money_used);
        }
    }
    else {
        if (!money_correct(money_used)) {
            points = -10;
        }
    }

#if USE_MEMO
    dp.memo[selected_item][money_used] = points? points : -1;
#endif
    return points;
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
#if USE_MEMO
            dp = {};
#endif
        }
        items.reserve(N);
        for (int i=0;i<N;++i) {
            arr = helper::num_array<int,2>(reader());
            items.emplace_back(std::make_pair(arr[0],arr[1]));
        }
        int total_points = solve(0,0);
        if (total_points<0) total_points=0;
        output.append(total_points).endl().flush();
    }
}

