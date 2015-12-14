#include "../../helper/helper.1.h"

std::string a,b,out;

std::string solve() {
    std::sort(a.begin(),a.end());
    std::sort(b.begin(),b.end());

    out.clear();

    std::for_each(a.begin(),a.end(),[](char c) {
                size_t pos = b.find(c);
                if (pos!=b.npos) {
                    out.push_back(c);
                    b[pos] = ' ';
                }
            });
    return out;
}

int main() {

    a.reserve(1000);
    b.reserve(1000);
    out.reserve(1000);

    helper::BufferedStdout output;
    helper::LineReader<> reader;

    while( reader.get_line(a) ) {
        reader.get_line(b);
        output.append(solve()).endl();
    }
}

