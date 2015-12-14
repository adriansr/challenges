#include "../../helper/helper.1.h"
#include <set>

Vi front,
   rear;

using FloatType = double;

FloatType solve() {
    std::multiset<FloatType> ratios;

    for ( FloatType front_ratio : front ) {
        for ( FloatType rear_ratio : rear) { 
            ratios.insert( rear_ratio / front_ratio);
        }
    }

    auto it  = ratios.cbegin(),
         end = ratios.cend();

    FloatType max_spread = 0,
          prev = *it++;
    while (it != end) {
        FloatType ratio = *it++;
        FloatType spread = ratio / prev;
        prev = ratio;
        
        if ( spread > max_spread) {
            max_spread = spread;
        }
    }

    return max_spread;
}

int main() {

    helper::LineReader<> reader;

    std::string line;

    helper::BufferedStdout output;
    char buf[100];

    while( reader.get_line(line) && line.size()>1 ) {
        reader.get_line(line);
        front = helper::num_vector<int>(line);
        reader.get_line(line);
        rear = helper::num_vector<int>(line);
        
        snprintf(buf,99,"%.02f\n",solve());
        output.append(buf);
    }
}

