#include "../../helper/helper.1.h"
int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;

    const int NUM_CASES = helper::string_to_int(reader());
    
    for (int ncase=0;ncase<NUM_CASES;++ncase) {
        reader(); // discard
        reader.get_line(line);

        size_t pos;
        char a,b;
        int num,
            min,
            max;

        a = line[0];
        b = line[1];

        if (b>='0') {
            min = max = (a-'0')*10+b-'0';
            pos = 3;
        }
        else {
            min = max = a-'0';
            pos = 2;
        }

        for(;pos<line.size();) {
            a = line[pos++];
            b = line[pos++];
            if (b >= '0') {
                num = (a-'0')*10+b-'0';
                ++pos;
            }
            else {
                num = a-'0';
            }
            if (num < min) {
                min = num;
            }
            else if(num > max) {
                max = num;
            }
        }

        output.append(2*(max-min)).endl();
    }
}

