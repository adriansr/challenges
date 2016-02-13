#include "../../helper/helper.1.h"
#include <algorithm>
#include <numeric>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096,
                 NUM_VALUES = 5,
                 NUM_OP_ARRANGEMENTS = 81,
                 TARGET_RESULT = 23;

using ProblemArray = std::array<int,NUM_VALUES> ;
using Operation = std::function<int(int,int)> ;

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

std::array<Operation,3> op { add,sub,mul };

bool evaluate(const ProblemArray& arr) {
    for (int i=0;i<NUM_OP_ARRANGEMENTS;++i) {
        int result = op[(i/27)](op[(i/9)%3](op[(i/3)%3](op[i%3](arr[0],arr[1]),arr[2]),arr[3]),arr[4]);
        if (result == TARGET_RESULT) {
            return true;
        }
    }
    return false;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;
    
    while ( reader.get_line(line) ) {
        ProblemArray problem = helper::num_array<int,NUM_VALUES>(line);
        if (std::all_of(problem.cbegin(),problem.cend(),[](int val) { return val == 0;}))
            break;

        std::sort(problem.begin(),problem.end());
        bool found;
        do {
            found = evaluate(problem);
        }
        while (!found && std::next_permutation(problem.begin(),problem.end()));

        if (found) {
            output.append("Possible\n");
        }
        else {
            output.append("Impossible\n");
        }
    }
}

