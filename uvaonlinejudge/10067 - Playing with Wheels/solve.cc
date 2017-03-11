#include "../../helper/helper.1.h"

constexpr size_t MAX_INPUT_LINE_LENGTH = 65536;

using code_t = uint16_t; // TODO: int32_t ?
constexpr code_t MAX_CODES = 0x999a;

code_t inline read_code(const char *ptr) {
#define DEC(CHR) ( (CHR) & 0xF )
#define EXTR(CHR,SHIFT) ( DEC(CHR) << SHIFT )
    return EXTR(ptr[0],12) 
         | EXTR(ptr[2], 8)
         | EXTR(ptr[4], 4)
         | EXTR(ptr[6], 0);
#undef EXTR
#undef DEC
}

std::bitset<MAX_CODES> visited;
std::array< std::array<bool, MAX_CODES>, 2> table;

constexpr int NOT_FOUND = -1;

constexpr char MAX_OP=8;
code_t next(code_t code, char op) {
    int shift = (op&0x3) << 2;
    bool add  = op&0x4;

    auto digit = (code>>shift) & 0xF;
    return add? ( digit != 9? code + (1 << shift)
                            : code & ~(0xF << shift) )
              : ( digit != 0? code - (1 << shift)
                            : (code & ~(0xF << shift)) | (9 << shift));
}

int solve(code_t source, code_t target) {
    char idx = 0;
    int  iters = 0;
    if (visited[target]) return NOT_FOUND;
    table[0][source] = true;
    visited[source] = true;

    std::cerr << std::hex;

    for (; !table[idx][target]; ++iters) {
        auto& tbl = table[idx];
        idx ^= 1;
        auto& ntbl = table[idx];
        ntbl.fill(false);
        for (code_t i=0;i<MAX_CODES;++i) {
            if (tbl[i]) {
                visited[i] = true;
                for (char op=0; op<MAX_OP; ++op) {
                    code_t code = next(i, op);
                    if (!visited[code]) {
                        ntbl[code] = true;
                    }
                }
            }
        }
    }
    table[idx].fill(false);
    return iters;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    std::string line;
    
    int NUM_TESTS = helper::string_to_int(reader());

    for (int test_num=0; test_num<NUM_TESTS; ++test_num) {
        if (test_num) {
            visited.reset();
            //table[0].fill(false);
            //table[1].fill(false);
        }

        code_t source = read_code(reader().data()),
               target = read_code(reader().data());

        int limits = helper::string_to_int(reader());

        for (int i=0; i<limits; ++i) {
            code_t lim = read_code(reader().data());
            visited.set(lim);
        }

        int result = solve(source, target);

        output.append(result).endl();
        output.flush();
    _exit(0);
    }
}

