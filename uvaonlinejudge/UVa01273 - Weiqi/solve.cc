#include "../../helper/helper.1.h"
#include <queue>
#include <cassert>
#include <unordered_set>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;
helper::BufferedStdout output;

class WeiqiGame {
    public:
        static constexpr int BOARD_SIZE = 19,
                             ROW_SIZE = BOARD_SIZE + 1;

        WeiqiGame() {
            reset();
        }

        enum PassType {
            Player,
            Invalid,
            Taboo
        };

        bool input(int y, int x) {
            ++turnNumber_;
            if (lastPassed_) {
                seen_boards_.clear();
            }

            player_ = other(player_);
            /*output.append(turnNumber_).append(": (").append(y).append(',').append(x)
                  .append(") ").append(player_).endl();*/
            if (x == -1 || y == -1) {
                return false;
            }

            if (first_) {
                ++gameCount_;
                output.append("Game #").append(gameCount_).endl();
                first_ = false;
            }
            if (x == 0 || y == 0) {
                onPass(Player,0,0);
                return true;
            }
            
            if (board_[pos(y-1,x-1)] != '+') {
                onPass(Invalid,y,x);
                return true;
            }
            
            lastPassed_ = put(y-1,x-1);
            if (!lastPassed_) {
                last_[player_=='e'] = std::make_pair(y,x);
                if (!seen_boards_.insert(board_).second) {
                    onEnd(true);
                }
            }
            //printBoard();
            //output.endl();
            return true;
        }

    private:
        static int pos(int y, int x) {
            return y*ROW_SIZE + x;
        }
        using BoardType = std::string;
        using MaskType = std::bitset<BOARD_SIZE*ROW_SIZE>;

        static char other(char p) {
            assert(p != '+');
            return p == 'e'? 'o' : 'e';
        }

        bool put(int y, int x) {
            board_[pos(y,x)] = player_;
            MaskType ignore,mask;
            int killed[2] {floodFill(y,x,player_,ignore),0};
            char c;
            
            if (y > 0 && (c=board_[pos(y-1,x)]) != '+') {
                bool idx = c != player_;
                if (idx) {
                    MaskType m;
                    killed[idx] += floodFill(y-1,x,c,m);
                    mask |= m;
                }
            }
            if (y < BOARD_SIZE && (c=board_[pos(y+1,x)]) != '+') {
                bool idx = c != player_;
                if (idx) {
                    MaskType m;
                    killed[idx] += floodFill(y+1,x,c,m);
                    mask |= m;
                }
            }
            if (x > 0 && (c=board_[pos(y,x-1)]) != '+') {
                bool idx = c != player_;
                if (idx) {
                    MaskType m;
                    killed[idx] += floodFill(y,x-1,c,m);
                    mask |= m;
                }
            }
            if (x < BOARD_SIZE && (c=board_[pos(y,x+1)]) != '+') {
                bool idx = c != player_;
                if (idx) {
                    MaskType m;
                    killed[idx] += floodFill(y,x+1,c,m);
                    mask |= m;
                }
            }

            if (killed[0]>0 && !killed[1]) {
                board_[pos(y,x)] = '+';
                onPass(Taboo,y+1,x+1);
                return true;
            }
            else if (killed[1]>0) {
                char enemy = other(player_);
                auto l = last_[enemy=='e'];
                if (killed[1] == 1 && l.first!=0 && mask[pos(l.first-1,l.second-1)]) {
                    /*std::cerr << turnNumber_ << ": taboo last "
                              << "killed " << l.first << ":" << l.second << '\n';
                    for (int j=0;j<BOARD_SIZE;++j) {
                        for (int i=0;i<BOARD_SIZE;++i) {
                            if (mask[pos(j,i)]) {
                                std::cerr << "mask " << (j+1) << "," << (i+1) << "\n";
                            }
                        }
                    }*/
                    onPass(Taboo,y+1,x+1);
                    board_[pos(y,x)] = '+';
                    return true;
                }
                killall(enemy,mask);
            }
            return false;
        }
        
        void killall(char c, const MaskType& mask) {
            for (int j=0;j<BOARD_SIZE;++j) {
                for (int i=0;i<BOARD_SIZE;++i) {
                    if (mask[pos(j,i)]) {
                        //std::cerr << "<" << board_[pos(j,i)] << ">\n";
                        assert(board_[pos(j,i)]==c);
                        board_[pos(j,i)] = '+';
                    }
                }
            }
        }

        int floodFill(int sy, int sx, char value, MaskType& mask) {
            std::queue<ii> q;
            q.emplace(sy,sx);
            int count = 0,x,y;
            do {
                std::tie(y,x) = q.front();
                q.pop();
                if (y>=0 && y<BOARD_SIZE && x>=0 && x<BOARD_SIZE) {
                    auto& b = board_[pos(y,x)];
                    if (!mask[pos(y,x)]) {
                        if (b == value) {
                            ++ count;
                            mask[pos(y,x)] = true;
                            q.emplace(y+1,x);
                            q.emplace(y-1,x);
                            q.emplace(y,x+1);
                            q.emplace(y,x-1);
                        }
                        else if(b == '+') {
                            /*if (turnNumber_ == 21) {
                                std::cerr << "Reset at " << (y+1) << "," << (x+1) 
                                    << " " << value << "\n";
                            }*/
                            mask.reset();
                            return 0;
                        }
                    }
                }
            }
            while(!q.empty());
            return count;
        }

        void onPass(PassType type, int y, int x) {

            output.append("Turn ").append(turnNumber_).append(' ')
                  .append(player_=='o'?"white":"black")
                  .append(" pass (");
            switch (type) {
                case Player:
                    output.append("by player");
                    break;
                case Invalid:
                    output.append("invalid put ");
                    break;
                case Taboo:
                    output.append("taboo point ");
                    break;
            }
            if (type != Player) {
                output.append(y).append(',').append(x);
            }
            output.append(")\n");

            if (lastPassed_) {
                // TODO: really? maybe check for a tie?
                onEnd(false);
            }
            lastPassed_ = true;
        }
        
        void printBoard() {
            /*std::array<char,BOARD_SIZE+1> line;
            line[BOARD_SIZE] = '\n';
            for (int j=0;j<BOARD_SIZE;++j) {
                for (int i=0;i<BOARD_SIZE;++i) {
                    line[i] = "+eo"[board_[j][i]];
                }
                output.append(line.data(),line.size());
            }*/
            output.append(board_);
        }

        void onEnd(bool tie) {
            output.append("Total ").append(turnNumber_).append(" turn(s)\n");
            if (tie) 
                output.append("(Tie)\n");
            printBoard();    
            reset();
        }

        void reset() {
            
            board_.clear();
            board_.resize(BOARD_SIZE*ROW_SIZE,'+');
            for (int i=0;i<BOARD_SIZE;++i) {
                board_[pos(i,BOARD_SIZE)] = '\n';
            }
            player_ = 'o';
            first_ = true;
            turnNumber_ = 0;
            lastPassed_ = false;
            seen_boards_.clear();
            last_[0] = {0,0};
            last_[1] = {0,0};
        }

        bool first_ = true;

        BoardType board_;
        char player_ = 'o';
        int gameCount_ = 0;
        int turnNumber_ = 0;
        bool lastPassed_ = false;
        std::unordered_set<BoardType> seen_boards_;
        ii last_[2];
};

class NumberPairReader {
public:
    NumberPairReader() {
    }

    bool get(int& a, int& b) {
        if (used_ + 2 > numbers_.size() && !fill() ) {
            return false;
        }
        a = numbers_[used_++];
        b = numbers_[used_++];
        return true;
    }


private:

    bool fill() {
        if (used_ == numbers_.size()) {
            numbers_ = helper::num_vector<int>(reader_());
        }
        else {
            numbers_.erase(numbers_.begin(),numbers_.begin()+used_);
            auto v = helper::num_vector<int>(reader_());
            numbers_.insert(v.begin(),v.end(),numbers_.end());
        }
        used_ = 0;
        return numbers_.size()>1;
    }

    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader_; // <-- check or RE
    Vi numbers_;
    size_t used_ = 0;
};

WeiqiGame game;
NumberPairReader npreader;

int main() {
    int a, b;
    while (npreader.get(a,b) && game.input(a,b)) {
    }
}

