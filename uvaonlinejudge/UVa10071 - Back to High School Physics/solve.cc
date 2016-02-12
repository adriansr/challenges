#include "../../helper/helper.1.h"

constexpr size_t MAX_INPUT_LINE_LENGTH = 40960;

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;
    
    std::array<int,2> params;
    while (reader.get_line(line)) {
        params = helper::num_array<int,2>(line);
        output.append(params[0]*params[1]*2).endl();
    }
}

