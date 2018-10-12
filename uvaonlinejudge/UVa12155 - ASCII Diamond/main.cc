#include "../../helper/helper.1.h"

constexpr size_t MAX_INPUT_LINE_LENGTH = 16384;
constexpr int MAX_OUTPUT_SIZE = 2 * 1024 * 1024;

std::array<char, MAX_OUTPUT_SIZE> screen;

int mirror(int v, int N) {
    return N - (v + 1 - N);
}

char diamond_at_(int x, int y, int size) {
    int d = x + y;
    if (d < size - 1) {
        return '.';
    }
    return 'a' + (mirror(d, size) - 1) % ('z' - 'a' + 1);
}

char diamond_at(int x, int y, int size) {
    if (x >= size) {
        x = mirror(x+1, size);
    }
    if (y >= size) {
        y = mirror(y+1, size);
    }
    return diamond_at_(x, y, size);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    
    for (int caseNum=1; ;caseNum++) {
        auto params = helper::num_array<int, 5>(reader());
        const int N = params[0];
        if (N == 0) break;
        const int row1 = params[1];
        const int col1 = params[2];
        const int row2 = params[3];
        const int col2 = params[4];
        
        const int rows = row2 - row1 + 1;
        const int cols = col2 - col1 + 1;
        const int rowSize = cols + 1;
        const int screenSize = rows * rowSize;
        
        char *ptr = &screen[0];
        for (int i=0;i<rows;i++) {
            for (int j=0;j<cols;j++) {
                int x = j + col1;
                int y = i + row1;
                *ptr++ = diamond_at(x % (2*N-1), y % (2 * N -1), N);
            }
            *ptr++ = '\n';
        }
        output.append("Case ")
              .append(caseNum)
              .append(":\n", 2)
              .append(screen.data(), screenSize);
    }
}

