#include "../../helper/helper.1.h"

std::map<Vi,bool> memo;

Vi stringToV(const std::string& s) {
    int count = 1,
        len = s.size();
    Vi result;
    result.reserve(s.size());
    char c = s[0];
    for (int i=1; i < len; ++i) {
        if (s[i] == c) {
            ++count;
        }
        else {
            c = s[i];
            result.push_back(count);
            count = 1;
        }
    }
    if (count)
        result.push_back(count);
    return result;
}

void erase_at(Vi& vect, int pos) {
    int lastpos = vect.size() - 1;
    if (pos>0 && pos<lastpos) {
        vect[pos-1] += vect[pos+1];
        vect.erase(vect.begin()+pos, vect.begin()+pos+2);
    }
    else
        vect.erase(vect.begin()+pos);
}

bool backtrack(const Vi& vect) {
    const auto it = memo.find(vect);
    if (it != memo.end()) {
        return it->second;
    }
    
    if (vect.empty())
        return true;

    bool retval = false;
    for (int i = 0, len = vect.size(); i < len; ++i) 
        if (vect[i]>1) {
            Vi tmp(vect);
            erase_at(tmp,i);
            if (backtrack(tmp)) {
                retval = true;
                break;
            }
        }
    memo[vect] = retval;
    return retval;
}

bool solve(const std::string& s) {
    if (s.size()) {
        Vi problem(stringToV(s));
        return backtrack(problem);
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

