 #include "../../helper/helper.1.h"

helper::BufferedStdout output;
helper::LineReader<1024*1024> reader;

constexpr int NUM_STATES = 52;
constexpr int RESERVE = (1024*1024) / 52;

std::array< std::vector<int> , NUM_STATES> list;

std::string line;
size_t len;

int state_id(char c) {
    return c <= 'Z'? c - 'A' : 'Z'-'A'+1+c-'a';
}

bool read_run(int *pstate, int *pcount, size_t *ppos) {
    size_t pos = *ppos;
    if ( pos < len ) {
        char id = line[pos++];
        int count;
        for (count = 1; pos<len && line[pos]==id; ++pos, ++count)
           ; 
        *pstate = state_id(id);
        *pcount = count;
        *ppos = pos;
        return true;
    }
    return false;
}

bool query(int *pmin, int *pmax) {
    len = line.size();
    //std::cerr << "line:" << line << "\n";
    int state,
        count;
    size_t pos = 0;
    int min,
        max;

    // read first 
    if ( read_run(&state,&count,&pos) ) {
        auto& plist = list[state];
        if ( plist.size() >= count ){
            min = plist[0];
            max = plist[count-1];
        }
        else
            return false;
    }
    else
        return false;
    
    // read rest
    while ( read_run(&state,&count,&pos) ) {
        auto &plist = list[state];
        const auto it = std::lower_bound(plist.cbegin(),plist.cend(),max);
        if ( it != plist.cend() ) {
            size_t limit = it - plist.cbegin() + count;
            if ( plist.size() >= limit ) {
                max = plist[limit-1];
            }
            else
                return false;
        }
        else
            return false;
    }
    
    *pmin = min;
    *pmax = max;
    return true;
}

int main() {

    for ( auto& elem : list) {
        elem.reserve(RESERVE);
    }

    int smin,smax;
    reader.get_line(line);
    for (int serial = 0, total = line.size(); serial < total ; ++serial) {
        list[ state_id(line[serial]) ].push_back(serial);
    }

    reader.get_line(line);
    const int Q = helper::string_to_int(line);

    for (int q=0; q<Q; ++q) {
        reader.get_line(line);
        if (query(&smin,&smax)) {
            output.append("Matched ",8)
                  .append(smin)
                  .append(' ')
                  .append(smax)
                  .endl();
        }
        else {
            output.append("Not matched\n",12);
        }
    }
}

