#include "../../helper/helper.1.h"

constexpr int MAX_BANKS = 20;

Vi reserves;
std::array<int,MAX_BANKS> debts;

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;

    std::array<int,2> params;
    std::array<int,3> info;
    
    reserves.reserve(MAX_BANKS);

    for(int ncase=0;;++ncase) {

        params = helper::num_array<int,2>(reader());

        const int num_banks = params[0],
                  num_debts = params[1];

        if (num_banks == 0)
            break;

        if (ncase) {
            reserves.clear();
            debts.fill(0);
        }

        reserves = helper::num_vector<int>(reader());

        int i;
        for (i=0;i<num_debts;++i) {
            info = helper::num_array<int,3>(reader());
            debts[info[0]-1] += info[2];
            reserves[info[1]-1] += info[2];
        }
        
        for(i=0;i<num_banks;++i) {
            if (debts[i] > reserves[i])
                break;
        }
        output.append( i == num_banks? 'S' : 'N').endl();
    }
}

