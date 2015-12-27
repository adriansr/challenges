#include "../../helper/helper.1.h"

Vi coins;

int solve() {
    int len = coins.size();
    if (len <= 2)
        return len;

    //std::sort(coins.begin(),coins.end());
    int ncoins = 2;
    int64_t used = coins[0];

    for (int len=coins.size()-1,i = 1;i<len;++i) {
        int val = coins[i];
        if (used+val < coins[i+1]) {
            used += val;
            ++ncoins;
        }
    }

    return ncoins;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    const int NUM_CASES = helper::string_to_int(reader());

    for (int case_num=0;case_num<NUM_CASES;++case_num) {

        const int NUM_COINS = helper::string_to_int(reader());

        if (case_num) {
            coins.clear();
        }
        coins.reserve(NUM_COINS);
        coins = helper::num_vector<int>( reader() );

        output.append(solve()).endl();
    }

}

