#include "../../helper/helper.1.h"
#include <cassert>
#include <unordered_map>

constexpr size_t MAX_INPUT_LINE_LENGTH = 16384;
static helper::LineReader<MAX_INPUT_LINE_LENGTH> reader;

constexpr int SHIFT = 5; // 5 bits per character
constexpr int MAX_DIGITS = 10;

class StringWindow {
public:

    StringWindow(int N, const char *str) 
    : value_(0)
    , mask_( (1L << (SHIFT*N)) - 1) {
        for (int i=0;i<N;++i) {
            value_ = (value_ << SHIFT) | char2index(str[i]);
        }
    }

    int64_t append(char c) {
        value_ = ((value_ << SHIFT) | char2index(c)) & mask_;
        return value_;
    }

    int64_t value() const {
        return value_;
    }

private:
    static int char2index(char c) {
        return c - 'a';
    }

    int64_t value_;
    const int64_t mask_;
};

static std::unordered_map<int64_t, int> substrings;
static char outputVector[MAX_DIGITS+1];

int main() {
    helper::BufferedStdout output;
    std::string line;
    
    while (reader.get_line(line)) {
        int N;
        size_t startPos;
        if (line[1] == ' ') {
            N = line[0] - '0';
            startPos = 2;
        } else {
            N = 10;
            startPos = 3;
        }
        for (;line[startPos] == ' ';++startPos)
            ;
        
        StringWindow acc(N, &line[startPos]);
        int maxCount = 1,
            maxValue = acc.value();

        substrings.clear();
        substrings[maxValue] = 1;
        for (size_t limit=startPos+N;limit < line.size(); ++limit) {
            auto value = acc.append(line[limit]);
            int candidate = ++substrings[value];
            if (candidate > maxCount) {
                maxCount = candidate;
                maxValue = value;
            }
        }
        for (int shift=(N-1)*SHIFT,i=0;i<N;shift-=SHIFT,++i) {
            outputVector[i] = 'a' + ((maxValue >> shift)&((1<<SHIFT))-1);
        }
        outputVector[N] = '\n';
        output.append(outputVector, N+1);
    }
}

