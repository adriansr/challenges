#include "../../../../helper/helper.1.h"

constexpr int MAX_N = 50;

int N,K;
std::array<std::array<char,MAX_N>,MAX_N> board;

void rotate_cw() {
    for (int y=0;y<N;++y) {
        int x,o;
        for (x=N-1,o=x;x>=0;--x) {
            if (board[y][x]) {
                board[y][o--] = board[y][x];
            }
        }
        for(;o>=0;--o)
            board[y][o]=0;
    }
}

class VictoryChecker {
    public:
        VictoryChecker(char player) : player_(player) {}
        void reset() {
            stride_ = 0;
        }
        bool feed(char piece) {
            if (piece == player_) {
                if (++stride_ == K)
                    return true;
            }
            else 
                reset();
            return false;
        }

    private:
        char player_;
        int  stride_;
};

bool wins(char player) {
    VictoryChecker v(player);
    // horizontal checks
    for (int y=0;y<N;++y) {
        for (int x=0;x<N;++x) {
            if (v.feed(board[y][x]))
                return true;
        }
        v.reset();
    }
    // vertical checks
    for (int x=0;x<N;++x) {
        for (int y=0;y<N;++y) {
            if(v.feed(board[y][x]))
                return true;
        }
        v.reset();
    }
    // top row, right-bound diagonals
    for (int i=0;i<N;++i) {
        int x=i,
            y=0;
        for (;x<N&&y<N;x++,y++) {
            if (v.feed(board[y][x]))
                return true;
        }
        v.reset();
    }
    // left column, downward-bound diagonals
    for (int i=1;i<N;++i) {
        int y=i,
            x=0;
        for (;x<N&&y<N;x++,y++) {
            if (v.feed(board[y][x]))
                return true;
        }
        v.reset();
    } 
    // top row, left-bound diagonals
    for (int i=0;i<N;++i) {
        int x=i,
            y=0;
        for (;x>=0&&y<N;x--,y++) {
            if (v.feed(board[y][x]))
                return true;
        }
        v.reset();
    }
    // right column, downward-bound diagonals
    for (int i=1;i<N;++i) {
        int y=i,
            x=N-1;
        for (;x>=0&&y<N;x--,y++) {
            if (v.feed(board[y][x]))
                return true;
        }
        v.reset();
    }
    return false;
}

std::string solve() {
    rotate_cw();
    bool red_wins = wins('R'),
         blue_wins = wins('B');

    if (red_wins || blue_wins) {
        if (red_wins && blue_wins)
            return "Both";
        return red_wins? "Red" : "Blue";
    }
    return "Neither";
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    const int NUM_TESTS = helper::string_to_int(reader());

    std::array<int,2> params;
    for (int ncase=1;ncase<=NUM_TESTS;++ncase) {
        params = helper::num_array<int,2>(reader());
        N = params[0],
        K = params[1];

        for (int y=0;y<N;++y) {
            reader.get_line(line);
            for (int x=0;x<N;++x) {
                board[y][x] = line[x]=='.'? 0 : line[x];
            }
        }

        output.append("Case #").append(ncase).append(": ")
              .append(solve()).endl();
    }
}

