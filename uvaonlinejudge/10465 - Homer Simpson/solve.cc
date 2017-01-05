#include "../../helper/helper.1.h"
#include <cassert>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;

std::pair<int, int> solve(int m, int n, int t, int b=0) {
    if (m>n) {
        std::swap(m, n);
    }
    // here m < n
    int km = t / m;
    if ((t % m) == 0) {
        return std::make_pair(km, b);
    }
    if (m != n) {
        while (--km >= 0) {
            int r = t - km * m;
            int kn = r / n;
            if ((r % n) == 0) {
                return std::make_pair(km + kn, b);
            }
        }
    }
    return solve(m, n, t-1, b+1);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;


    while (reader.get_line(line)) {
        auto params = helper::num_array<int, 3>(line);
        auto result = solve(params[0], params[1], params[2]);
        output.append(result.first);
        if (result.second)
            output.append(' ').append(result.second);
        output.endl();
    }
}

