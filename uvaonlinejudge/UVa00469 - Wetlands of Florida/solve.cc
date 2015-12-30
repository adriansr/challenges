#include "../../helper/helper.1.h"
#include <queue>

#define MAX_SIZE 100

int WIDTH, HEIGHT;
std::array<std::array<char,MAX_SIZE>,MAX_SIZE> map;

int solve(int basex, int basey) {
    std::queue<std::pair<int,int>> queue;

    queue.emplace(basex,basey);
    
    char target = map[basey][basex],
         visited = target=='W'? 'w' : 'W';

    int area = 0;
    do {
        int x,y;
        std::tie(x,y) = queue.front();
        queue.pop();

        if (x>=0 && x<WIDTH && y>=0 && y<HEIGHT && map[y][x]==target) {
            ++area;
            map[y][x] = visited;
            queue.emplace(x+1,y);
            queue.emplace(x+1,y+1);
            queue.emplace(x+1,y-1);
            queue.emplace(x,y+1);
            queue.emplace(x,y-1);
            queue.emplace(x-1,y+1);
            queue.emplace(x-1,y);
            queue.emplace(x-1,y-1);
        }
    }
    while( !queue.empty() );
    return area;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::array<int,2> query;

    const int NUM_CASES = helper::string_to_int( reader() );
    reader.get_line(line);
    for (int ncase = 0; ncase < NUM_CASES; ++ncase) {

        reader.get_line(line);
        WIDTH = line.size();
        int y;
        for(y=0;line[0]=='L' || line[0]=='W';++y) {
            for (int x=0;x<WIDTH;++x) {
                map[y][x] = line[x];
            }
            reader.get_line(line);
        }
        HEIGHT = y;
        
        if (ncase) 
            output.endl();

        do {
            query = helper::num_array<int,2>(line);
            output.append(solve(query[1]-1,query[0]-1)).endl();
        }
        while (reader.get_line(line) && !line.empty());
    }
}

