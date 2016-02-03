#include "../../helper/helper.1.h"

int main() {
    helper::BufferedStdout output;
    helper::LineReader<256*1024> reader;
    std::string line;

    const int num_cases = helper::string_to_int(reader());

    for (int case_num=0; case_num<num_cases ; ++case_num) {
        reader();

        Vi permutation = helper::num_vector<int>(reader());
        line = reader();   
        
        std::vector<ii> floats;
        floats.reserve(permutation.size());
        helper::for_each_word_predicate<>(line,[&floats](std::string&, size_t start, size_t len) {
                    floats.emplace_back(start,len);
                });
        
        if (case_num)
            output.endl();
        
        std::vector<ii> moved;
        moved.resize(floats.size());
        for (int pos = 0;pos<permutation.size();++pos) {
            moved[permutation[pos]-1] = floats[pos];
        }
        for (const auto s : moved) {
            output.append(&line[s.first],s.second).endl();
        }
    }
}

