#include "../../helper/helper.1.h"
#include <unordered_map>

constexpr size_t MAX_INPUT_LINE_LENGTH = 1000000;
using TypeReader = helper::LineReader<MAX_INPUT_LINE_LENGTH>;
static TypeReader reader;

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

template<class Reader>
class MixedInputReader {
public:
    MixedInputReader(Reader& r): reader_(r), pos_(0) {
    }

    bool next_problem(int *pN, const char **pStart, const char **pEnd) {
        return getN(pN) && getS(pStart, pEnd);
    }

private:

    bool advance() {
        while (pos_ < line_.size() && line_[pos_] == ' ') ++pos_;
        while (pos_ >= line_.size()) {
            pos_ = 0;
            bool result = reader_.get_line(line_);
            if (!result) return false;
            while (line_[pos_] == ' ') ++pos_;
        }
        return true;
    }

    bool getN(int *pN) {
        bool rv = advance();
        if (rv) {
            int val = line_[pos_++] - '0';
            if (val == 1 && pos_ < line_.size() && line_[pos_]=='0') {
                val = 10;
                ++ pos_;
            }
            *pN = val;
        }
        return rv;
    }

    bool getS(const char **pStart, const char **pEnd) {
        bool rv = advance();
        if (rv) {
            *pStart = line_.data() + pos_;
            //std::cerr << "line '" << line_ << "' pos:" << pos_ << '\n';
            while (pos_ < line_.size() && line_[pos_] >= 'a') ++pos_;
            *pEnd = line_.data() + pos_;
        }
        return rv;
    }

    Reader& reader_;
    std::string line_;
    size_t pos_;
};

std::array<int, 26> one_counts;

int main() {
    helper::BufferedStdout output;
    
    MixedInputReader<TypeReader> mixedReader(reader);

    int N;
    const char *str, *end;
    while (mixedReader.next_problem(&N, &str, &end)) {
        
#define OPTIMIZE_FOR_N1     false // no improvement. No N=1 cases in judge?
#define OPTIMIZE_MAX_SEARCH false // no improvement. Why?

        if (!OPTIMIZE_FOR_N1 || N > 1) {
            StringWindow acc(N, str);
            int maxCount = OPTIMIZE_MAX_SEARCH? 0 : 1;
            int64_t maxValue = acc.value();
            
            substrings.clear();
            substrings[maxValue] = 1;
            for (str += N; str < end; ++str) {
                auto value = acc.append(*str);
                int candidate = ++substrings[value];
                if (!OPTIMIZE_MAX_SEARCH && candidate > maxCount) {
                    maxCount = candidate;
                    maxValue = value;
                }
            }
            
            if (OPTIMIZE_MAX_SEARCH) {
                for (auto it = substrings.cbegin()
                    ;it != substrings.cend()
                    ;++it) {
                        if (it->second > maxCount) {
                            maxCount = it->second;
                            maxValue = it->first;
                        }
                }
            }

            for (int shift=(N-1)*SHIFT,i=0;i<N;shift-=SHIFT,++i) {
                outputVector[i] = 'a' + ((maxValue >> shift)&((1<<SHIFT) - 1) );
            }
        } else {
            one_counts.fill(0);
            for (; str < end; ++str) {
                ++ one_counts[*str - 'a'];
            }
            int maxVal = one_counts[0];
            outputVector[0] = 0;
            for (int i=1;i<26;++i) {
                if (one_counts[i] > maxVal) {
                    maxVal = one_counts[i];
                    outputVector[0] = i;
                }
            }
            outputVector[0] += 'a';
        }

        outputVector[N] = '\n';
        output.append(outputVector, N+1);
    }
}

