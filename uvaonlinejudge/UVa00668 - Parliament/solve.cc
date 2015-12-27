#include "../../helper/helper.1.h"

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    
    const int NUM_CASES = helper::string_to_int(reader());

    for (int case_num=0;case_num<NUM_CASES;++case_num) {
        
        reader();
        int n = helper::string_to_int(reader());
        Vi groups;

        for (int candidate=2; candidate<=n; ++candidate) {
            groups.push_back(candidate);
            n -= candidate;
        }
        
        if (n) {
            if (!groups.empty())
                groups.back() += n;
            else
                groups.push_back(n);
        }
        
        int len = groups.size();

        bool changed ;
        do {
            changed = false;

            for (int pos=len-1;pos>0 && !changed;--pos) {
                int selected = groups[pos],
                    target_dec = selected - 1;
                for (int dest=0;dest<pos;++dest) {
                    int target_inc = groups[dest] + 1;
                    if (target_inc < groups[dest+1] && target_inc!=target_dec) {
                        int other_pos;
                        for (other_pos=0;other_pos<len;++other_pos) {
                            if (groups[other_pos] == target_inc
                             || groups[other_pos] == target_dec)
                                break;
                        }
                        if (other_pos == len) {
                            groups[pos] = target_dec;
                            groups[dest] = target_inc;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
        while (changed);

        if (case_num) {
            output.endl();
        }

        output.append(groups[0]);
        for (int i=1;i<len;++i) {
            output.append(' ').append(groups[i]);
        }
        output.endl();
    }

}

