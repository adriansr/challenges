#include "../../helper/helper.1.h"

/* This problem didn't make sense to me.
 * The fact that Bob plays OPTIMALLY doesn't mean he's going to pick up the smallest
 * number on every row.
 * He might pick a not-so smallest number to strike out a very large number on a different
 * row.
 * Am I wrong?
 */
#include <cstring>
#include <bitset>
#include <algorithm>

constexpr int MAX_SIZE = 8,
              INF = 100000;

int N;
std::array<int,MAX_SIZE*MAX_SIZE> board;
std::array<int,MAX_SIZE> state;

int solve() {
    if (N==1) return board[0];

    int best = INF;

    for (int i=0;i<N;++i) {
        state[i] = i;
    }
    
    do {
        int game = 0;
        for (int idx = 0; idx < N; ++idx) {
            game += board[ (idx*N) + state[idx] ];
        }

        if (game < best) {
            best = game;
        }
    }
    while (std::next_permutation(state.begin(),state.begin()+N));

    return best;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int NUM_TESTS = helper::string_to_int(line);

    for (int test_num = 0; test_num < NUM_TESTS; ++test_num) {

        reader.get_line(line);
        N = helper::string_to_int(line);
        
        for (int i=0, dest=0; i<N; ++i) {
            reader.get_line(line);
            helper::for_each_word_predicate(line,
                    [&dest](const std::string& s,size_t start, size_t end) {
                        board[dest++] = strtol(s.c_str()+start,nullptr,10);
                    });

        }
        output.append(solve()).append('\n');
    }
}

