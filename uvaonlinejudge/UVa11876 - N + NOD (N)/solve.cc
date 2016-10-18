#include "../../helper/helper.1.h"

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096,
                 LIMIT = 1000000,
                 SEQ_SIZE = 64725;

Vi seq;

std::array<int,LIMIT> nod;
void calc_nod() {

    for (int i=0;i<3;++i) 
        nod[i] = i;
    for (int i=3;i<LIMIT;++i) {
        nod[i] = (i&1)?2:3;
    }

    for (int i=3;i<LIMIT;++i) {
        for (int j=i+i;j<LIMIT;j+=i) {
            nod[j] ++;
        }
    }
}

void calc_seq() {
    seq.reserve(SEQ_SIZE);
    for (int val=1;val<LIMIT;val+=nod[val]) {
        seq.push_back(val);
    }
}

int solve(int min, int max) {
    auto begin = std::lower_bound(seq.cbegin(),seq.cend(),min),
         end   = std::upper_bound(seq.cbegin(),seq.cend(),max);
    return end - begin;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    
    calc_nod();
    calc_seq();
    
    const int num_tests = helper::string_to_int(reader());
    std::array<int,2> params;
    for (int case_num=1;case_num<=num_tests;++case_num) {
        params = helper::num_array<int,2>(reader());
        int count = solve(params[0],params[1]);
        output.append("Case ",5).append(case_num).append(": ",2).append(count).endl();
    }
}

