#include "../../helper/helper.1.h"
#include <unordered_map>
std::unordered_map<std::string,bool> memo;

std::string stringToV(const std::string& s) {
    int count = 1,
        len = s.size();
    std::string result;
    result.reserve(s.size());
    char c = s[0];
    for (int i=1; i < len; ++i) {
        if (s[i] == c) {
            ++count;
        }
        else {
            c = s[i];
            result.push_back(count>1? '1' : '0');
            count = 1;
        }
    }
    if (count)
        result.push_back(count>1? '1':'0');
    return result;
}

std::string copy_erase_at(const std::string& vect, int pos) {
    std::string retval;
    int len = vect.size();
    retval.reserve(len);

    for (int i = 0; i < pos;++i) {
        retval.push_back(vect[i]);
    }
    int from;
    if (pos>0 && pos+1 < len) {
        retval.back() = '1';
        from = pos + 2;
    }
    else
        from = pos + 1;

    for (int i=from; i < len; ++i) {
        retval.push_back(vect[i]);
    }

    return retval;
}

bool backtrack(std::string&& vect) {
    const auto it = memo.find(vect);
    if (it != memo.end()) {
        return it->second;
    }
    
    if (vect.empty())
        return true;

    bool retval = false;
    for (int i = 0, len = vect.size(); i < len; ++i) 
        if (vect[i]!='0') {
            if (backtrack(copy_erase_at(vect,i))) {
                retval = true;
                break;
            }
        }
    memo[vect] = retval;
    return retval;
}

bool solve(const std::string& s) {
    if (s.size()) {
        return backtrack(stringToV(s));
    }
    return true;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int NUM_CASES = helper::string_to_int(line);

    char result[2] = {'0','\n'};
    for (int ncase = 1; ncase <= NUM_CASES; ++ncase) {
        reader.get_line(line);
        result[0] = solve(line)? '1' : '0';
        output.append(result,2);
    }
}

