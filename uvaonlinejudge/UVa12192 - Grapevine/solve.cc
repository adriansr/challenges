#include "../../helper/helper.1.h"
constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;

helper::BufferedStdout output;
helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
std::string line;

constexpr int MAX_DIM = 500;
constexpr int MAX_DIAGONALS = MAX_DIM * 2 - 1;
int M, N;

std::array<std::vector<int>, MAX_DIAGONALS> diagonals;

int bsearch(const std::vector<int>& d, int start, int end, int value) {
    int a = start,
        b = end;
    if (d[a] >= value) return a;
    if (d[b] < value) return b+1;
    while (a < b) {
        int c = (a + b) / 2;
        if (c == a) break;
        (d[c] < value? a : b) = c;
    }
    return a;
}

int diagonal(const std::vector<int>& d, int best, int lower, int higher) {
    if (d.size() <= best) return 0;
    int lb = bsearch(d, 0, d.size()-1, lower);
    if (lb >= d.size()) return 0;
    int ub = bsearch(d, lb, d.size()-1, higher + 1);
    while (d[lb] < lower && lb <= ub && lb < d.size()) ++lb;
    while ((ub >= d.size() || d[ub] > higher) && lb <= ub) --ub;
    if (lb <= ub) {
        return ub - lb + 1;
    }
    return 0;
}

int solve(int lower, int higher) {
    int best = 0;
    const int lim = M + N - 1;
    for (int i=0; i<lim; ++i) {
        best = std::max(best, diagonal(diagonals[i], best, lower, higher));
    }
    return best;
}

int main() {
    for (;;) {
        auto params = helper::num_array<int, 2>(reader());
        M = params[0];
        N = params[1];
        if (M == 0 && N == 0) break;
        for (int i = 0; i < M+N-1; ++i) {
            diagonals[i].clear();
        }
        for (int y = 0; y < M; ++y) {
            auto v = helper::num_vector<int>(reader());
            for (int x = 0; x < N; ++x) {
                diagonals[x + M - y - 1].push_back(v[x]);
            }
        }
        int queries = helper::string_to_int(reader());
        for (int i = 0; i < queries; ++i) {
            params = helper::num_array<int, 2>(reader());
            int result = solve(params[0], params[1]);
            output.append(result).endl();
        }
        output.append("-\n", 2);
    }
}

