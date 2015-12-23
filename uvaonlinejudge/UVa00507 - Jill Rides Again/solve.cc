#include "../../helper/helper.1.h"

constexpr int INF = 1000000000;

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int NUM_ROUTES = helper::string_to_int(line);

    for (int nroute = 1; nroute <= NUM_ROUTES; ++nroute) {
        reader.get_line(line);
        const int NUM_STOPS = helper::string_to_int(line);

        int max_value = -INF,
            max_begin = 0,
            max_end = 0,
            max_len = -1,
            sum = 0,
            last_begin = 0;

        for (int i=0;i<NUM_STOPS-1;++i) {
            reader.get_line(line);
            sum += helper::string_to_int(line);
            if (sum < 0) {
                sum = 0;
                last_begin = i+1;
            }

            if (sum >= max_value) {
                int len = i - last_begin;
                if (sum>max_value || len>max_len) {
                    max_begin = last_begin;
                    max_end = i;
                    max_len = len;
                    max_value = sum;
                }
            }
        }
        
        if (max_value!=0) {
            output.append("The nicest part of route ")
                  .append(nroute)
                  .append(" is between stops ")
                  .append(max_begin + 1)
                  .append(" and ")
                  .append(max_end + 2)
                  .endl();
        }
        else { 
            output.append("Route ")
                  .append(nroute)
                  .append(" has no nice parts\n");
        }
    }
}

