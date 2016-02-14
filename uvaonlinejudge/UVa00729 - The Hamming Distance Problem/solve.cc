#include "../../helper/helper.1.h"

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096,
                 MAX_LEN = 16;

helper::BufferedStdout output;
int H, N;

std::array<char,MAX_LEN> buffer;

void solve(int pos, int needed) {
    int left = N - pos;
    if (pos < N) {
        if (left > needed) {
            buffer[pos] = '0';
            solve(pos+1,needed);
        }
        if (needed) {
            buffer[pos] = '1';
            solve(pos+1,needed-1);
        }
    }
    else {
        output.append(buffer.data(),N).endl();
    }
}

int main() {
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;
    
    const int num_cases = helper::string_to_int(reader());

    std::array<int,2> params;
    for (int case_num=0;case_num<num_cases;++case_num) {
        reader();
        if (case_num) {
            output.endl();
        }
        params = helper::num_array<int,2>(reader());
        N = params[0];
        H = params[1];
        solve(0,H);
    }
}

