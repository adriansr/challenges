#include "../../../../helper/helper.1.h"
#include <iostream>
#include <set>
#include <cassert>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096,
                 INF = 1000000001;

char inline reverse(char c) {
    return c ^ 6;
}

std::string line;
std::map<std::string,int> memo;

void flip(int pos) {
    int base;
    for (base=0;base<pos;++base,--pos) {
        char a = reverse(line[base]),
             b = reverse(line[pos]);
        line[base] = b;
        line[pos]  = a;
    }
    if (base == pos) {
        line[base] = reverse(line[base]);
    }
}

bool solved() {
    return line.find('-') == line.npos;
}

int solve_impl(int best_sofar) {
    int best = INF;
    char last = '+';
    
    if (solved()) {
        return 0;
    }

    if (false)//best_sofar==0)
        return INF;
    /*
     *
--+-
+-++
--++
++++
     */

    auto it = memo.find(line);
    if (it != memo.end()) {
        return it->second;
    }
    memo[line] = INF;

    for (int i=line.size()-1;best>1 && i>=0;--i) {
        if (line[i] != last) {
            last = line[i];
            //auto saved = line;
            flip(i);
            int val = solve_impl(best-1);
            //std::cerr << "flip " << i << " " << saved << ": " << line << " " << val << "/" << best << "\n";
            if (++val < best) {
                best = val;
            }
            flip(i);
            //assert(line == saved);
        }
    }
    
    memo[line] = best;
    //std::cerr << "return " << line << ": " << best << "\n";
    return best;
}

int solve() {
    return solve_impl(INF);
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    
    const int NUM_CASES = helper::string_to_int(reader());

    for (int case_num=1;case_num<=NUM_CASES;++case_num) {
        reader.get_line(line);
        output.append("Case #").append(case_num).append(": ").append(solve()).endl();
    }
}

