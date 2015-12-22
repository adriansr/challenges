#include "../../helper/helper.1.h"

/*std::pair<int,int> rsq(const Vi& s, int *pvalue) {
    int max_sum = -1;
    int max_pos = 0;

    for (int i = 0, sum = 0, len = s.size()
        ; i < len
        ; ++i) {

        sum += s[i];
        if (sum < 0) {
            sum = 0;
        }
        if (sum > max_sum) {
            max_sum = sum;
            max_pos = i;
        }
    }

    int max_start,sum;
    for (max_start=max_pos, sum=max_sum; sum>0 && max_start>=0; --max_start) {
        sum -= s[max_start];
    }

    *pvalue = max_sum;
    return std::make_pair(max_start,max_pos);
}*/

std::pair<int,int> solve(const Vi& s, int *pvalue) {
    Vi sS;
    sS.reserve(s.size());
    for (int i = 0, sum = 0; i < s.size(); ++i) {
        sum += s[i];
        sS.push_back(sum);
    }
    helper::report("route",s);
    helper::report("sums",sS);
    *pvalue = 0;
    return std::make_pair(0,0);
}


int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int NUM_ROUTES = helper::string_to_int(line);

    for (int nroute = 1; nroute <= NUM_ROUTES; ++nroute) {
        reader.get_line(line);
        const int NUM_STOPS = helper::string_to_int(line);

        Vi route;
        route.reserve(NUM_STOPS);

        for (int i=0;i<NUM_STOPS-1;++i) {
            reader.get_line(line);
            route.push_back(helper::string_to_int(line));
        }
        
        int sum;
        const auto result = solve(route,&sum);

        if (result.second != 0 && sum > 0) {
            output.append("The nicest part of route ")
                  .append(nroute)
                  .append(" is between stops ")
                  .append(result.first + 1)
                  .append(" and ")
                  .append(result.second + 2)
                  .endl();
        }
        else { 
            output.append("Route ")
                  .append(nroute)
                  .append(" has no nice parts\n");
        }
    }
}

