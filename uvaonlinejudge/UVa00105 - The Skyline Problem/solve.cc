#include "../../helper/helper.1.h"
#include <cstring>
#include <cstdio>

constexpr int MAX_DIM = 10000;
std::array<int,MAX_DIM> height_map;

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
 
    std::array<int,3> triplet;
    
    int max_x = -1;

    // create height map from input
    while( reader.get_line(line) ) {
        triplet = helper::num_array<int,3>(line);
        const int L = triplet[0],
                  H = triplet[1],
                  R = triplet[2];

        if (R > max_x)
            max_x = R;

        for (int x = L; x < R; ++x) {
            if (height_map[x] < H)
                height_map[x] = H;
        }
    }
    
    // output heightmap in fancy format
    int base = 0,
        x = 0,
        i = 0;

    do {
        for(;x<max_x && height_map[x] == base;++x) {
        }

        base = height_map[x];
        
        if (i)
            output.append(' ');
        output.append(x).append(' ').append(base);
    }
    while (++i,x < max_x);

    output.endl();
}

