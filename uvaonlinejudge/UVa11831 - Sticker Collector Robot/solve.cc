#include "../../helper/helper.1.h"

constexpr int MAX_DIM = 101;
std::array< std::array<char,MAX_DIM> , MAX_DIM> arena;

int N,
    M,
    S,
    total_stickers,
    current_dir;

struct Pos {
    Pos() {}
    constexpr Pos(int x, int y) : x(x), y(y) {}

    int x,y;

    char valid() const {
        
        if (x>=0 && x<M && y>=0 && y<N) {
           char c = arena[y][x];
           if (c!='#') {
               arena[y][x] = '.';
               return c;
           }
        }
        return 0;
    }

    Pos& operator+=(const Pos& p) {
        x += p.x;
        y += p.y;
        return *this;
    }
};

Pos operator+(const Pos& a, const Pos& b) {
    Pos new_pos(a);
    new_pos += b;
    return new_pos;
}

Pos robot;

const std::string dir_label("NLSO");
constexpr std::array<Pos,4> dirs {
    Pos(0,-1), // N
    Pos(1,0),  // L(E)
    Pos(0,1),  // S
    Pos(-1,0)  // O(W)
};

int solve(const std::string& instructions) {
    int num_stickers = 0;
    for (size_t ip=0; ip < S && num_stickers < total_stickers; ++ip) {
        switch( instructions[ip] ) {
        case 'D':
            current_dir = (current_dir+1)&3;
            break;
        case 'E':
            current_dir = (current_dir-1)&3;
            break;
        case 'F': {
                Pos new_pos = robot + dirs[current_dir];
                char item;
                if ( (item=new_pos.valid()) ) {
                    robot = new_pos;
                    if (item == '*') {
                        ++ num_stickers;
                    }
                }
            }
            break;
        }
    }
    return num_stickers;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<65536> reader;
    std::string line;
    
    std::array<int,3> params;
    for (int ncase=0; reader.get_line(line) ; ++ncase) {
        params = helper::num_array<int,3>(line);
        N = params[0];
        M = params[1];
        S = params[2];
        if (!N)
            break;
        total_stickers = 0;
        for (int i=0;i<N;++i) {
            reader.get_line(line);
            for (int j=0;j<M;++j) {
                char c = line[j];
                switch(c) {
                    case '*':
                        ++total_stickers;
                        break;
                    case '.':
                    case '#':
                        break;
                    default: // N, S, L(E), O(W)
                        current_dir = dir_label.find(c);
                        c = '.';
                        robot.x = j;
                        robot.y = i;
                }
                arena[i][j] = c;
            }
        }
        reader.get_line(line);
        output.append( solve(line) ).endl();
    }
}

