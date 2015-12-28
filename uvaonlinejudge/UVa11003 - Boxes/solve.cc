#include "../../helper/helper.1.h"

const int MAX_WEIGHT = 3001;
int N;

struct Box {
    Box(int weight, int maxload) 
        : weight(weight), maxload(maxload) {}

    int weight,
        maxload;
};

std::vector<Box> items;

int r[2][MAX_WEIGHT+2];

int solve() {
    if (N==0) return 0;
    constexpr int INF_WEIGHT = MAX_WEIGHT + 1;

    r[0][INF_WEIGHT] = 1;
    r[0][items[0].maxload] = 2;

    int prev = 0,
        current = 1;
    for (int box = 1; box < N; ++box) {
        for (int load = INF_WEIGHT; load>=0; --load) {
            int stack_size = r[prev][load];
            if (stack_size) {
#define MERGE(DEST,VAL) DEST = std::max(DEST,VAL)
                MERGE(r[current][load],stack_size);
                int remain = load - items[box].weight;
                if (remain>=0) {
                    int next_load = load!=INF_WEIGHT?
                            std::min(items[box].maxload,remain)
                            : items[box].maxload;
                    MERGE(r[current][next_load],1+stack_size);
                }
#undef MERGE
            }
        }
        std::swap(prev,current);
    }

    int best_stack = 1;
    for (int load = 0; load < INF_WEIGHT; ++load) {
        int val = r[prev][load];
        if (val > best_stack)
            best_stack = val;
    }
    return best_stack - 1;
}

helper::BufferedStdout output;
helper::LineReader<> reader;

int main() {
    std::array<int,2> arr;
    std::string line;
    for (int test_num=0; reader.get_line(line) ; ++test_num) {
        N = helper::string_to_int(line);
        if (N == 0) 
            break;

        if (test_num) {
            items.clear();
            memset(r,0,sizeof(r));
        }
        items.reserve(N);
        for (int i=0;i<N;++i) {
            arr = helper::num_array<int,2>(reader());
            items.emplace_back(Box(arr[0],arr[1]));
        }
        output.append(solve()).endl();
    }
}

