#include "../../helper/helper.1.h"
#include <limits>
#include <algorithm>
#include <cassert>
#include <numeric>

constexpr int //MAX_N = 1001,
              INF = std::numeric_limits<int>::max();

int N, M;
Vi sequence;
///td::array<std::array<int,MAX_N>,MAX_N> memo;

/*
 * slow full-search + dp solution
 *
int solve(int remain, int pos) {
    if (pos>=N)
        return INF;
    int &saved = memo[pos][remain];
    if (saved != INF)
        return saved;

    if (remain==1) {
        int sum = 0;
        while (pos < N) sum += sequence[pos++];
        saved = sum;
        return sum;
    }
    int sum = 0,
        best = INF,
        limit = N-remain+1;
    while (pos<limit) {
        sum += sequence[pos];
        int val = std::max(sum,solve(remain-1,++pos));
        best = std::min(best,val);
    }
    saved = best;
    return best;
}
*/

template<typename T, class Fn>
T binary_search(T low, T high, Fn f) {
    while (low + 1< high) {
        int mid = (low + high) / 2;
        if (f(mid)) {
            high = mid;
        }
        else
            low = mid;
    }
    return high;
}

// try making segments of size at most the max element
bool has_greedy_solution(int max_value) {
    int partial_sum = 0, count_segments = 1;
    for (int elem : sequence) {
        if (partial_sum+elem <= max_value) {
            partial_sum += elem;
        }
        else {
            partial_sum = elem;
            if (++count_segments > M)
                return false;
        }
    }
    if (partial_sum == 0)
        -- count_segments;

    return count_segments <= M;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<256*1024> reader;
    std::string line;
    
    std::array<int,2> problem;
    for(bool first=true; reader.get_line(line) ;first=false) {

        problem = helper::num_array<int,2>(line);
        N = problem[0];
        M = std::min(problem[1],N);
        
        sequence.clear();
        sequence.reserve(N);
        sequence = helper::num_vector<int>(reader());

        int min_result = *std::max_element(sequence.begin(),sequence.end())-1,
            max_result =  std::accumulate(sequence.begin(),sequence.end(),0),
                result =  binary_search(min_result,max_result,
                                           [](int pos) { return has_greedy_solution(pos); });
        /*if (N>M || has_greedy_solution(result)) {
            for (int i=0;i<N;++i) {
                memo[i].fill(INF);
            }
            result = solve(M,0);
        }*/
        /*while (!has_greedy_solution(result))
            ++ result;*/

        output.append(result).endl();
    }
}

