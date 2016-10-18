#include "../../../../helper/helper.1.h"
#include <iostream>
#include <set>
#include <cassert>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096,
                 INF = 1000000001;

char inline reverse(char c) {
    return c ^ 6;
}

std::map<std::string,int> memo;

std::string flip(const std::string& s, int pos) {
    auto line = s;
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
    return line;
}

bool solved(const std::string& line) {
    return line.empty();
}

std::string normalize(const std::string& s) {
    if (s.back()=='+') {
        auto pos = s.rfind('-');
        if (pos != s.npos) {
            return s.substr(0,pos+1);
        }
        return std::string();
    }
    return s;
}

int solve_impl(const std::string& line) {
    int best = INF;
    char last = '+';

    if (solved(line)) {
        return 0;
    }

    auto it = memo.find(line);
    if (it != memo.end()) {
        return it->second;
    }
    memo[line] = INF;
    if ((memo.size() & 0xffff) == 0) {
        std::cerr << memo.size() << '\n';
    }

    for (int i=line.size()-1;best>1 && i>=0;--i) {
        if (line[i] != last) {
            last = line[i];
            auto newline = flip(line,i);
            if (i == line.size()-1) {
                newline = normalize(newline);
            }
            int val = solve_impl(newline);
            if (++val < best) {
                best = val;
            }
        }
    }
    
    memo[line] = best;
    //std::cerr << "return " << line << ": " << best << "\n";
    return best;
}

int solve(const std::string& line) {
    return solve_impl(normalize(line));
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;
    
    const int NUM_CASES = helper::string_to_int(reader());

    for (int case_num=1;case_num<=NUM_CASES;++case_num) {
        reader.get_line(line);
        output.append("Case #").append(case_num).append(": ").append(solve(line)).endl();
    }
}

