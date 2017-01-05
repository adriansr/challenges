#include "../../helper/helper.1.h"
#include <cassert>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;

int find_tile(int value, const std::vector<int>& vb, int pb) {
    for (int p=pb; p<vb.size(); ++p) {
        if (vb[p] == value)
            return p;
    }
    return -1;
}

int solve(const std::vector<int>& va, const std::vector<int>& vb,
          int pa = 0, int pb = 0) {

    if (pa == va.size()) {
        return 0;
    }

    int pos = find_tile(va[pa], vb, pb);
    int resultTaken = pos != -1? 1 + solve(va, vb, pa + 1, pos + 1) : 0;
    int resultNot = solve(va, vb, pa + 1, pb);

    return std::max(resultTaken, resultNot);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    
    for (int N=1;;++N) {
        auto params = helper::num_array<int, 2>(reader());
        if (params[0] == 0 && params[1] == 0) break;
        auto v1 = helper::num_vector<int>(reader());
        assert(v1.size() == params[0]);
        auto v2 = helper::num_vector<int>(reader());
        assert(v2.size() == params[1]);
        
        auto result = v1.size() < v2.size()? solve(v1, v2) : solve(v2, v1);

        output.append("Twin Towers #", 13).append(N).endl()
              .append("Number of Tiles : ", 18).append(result).endl()
              .endl();
    }
}

