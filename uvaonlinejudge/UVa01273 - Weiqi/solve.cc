#include "../../helper/helper.1.h"
#include <queue>
#include <cassert>
#include <unordered_set>

constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;
helper::BufferedStdout output;

class WeiqiGame {
    public:
        static constexpr int BOARD_SIZE = 19;

        WeiqiGame() {
            reset();
        }

        enum Color {
            None,
            Black,
            White
        };

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
            output.append(turnNumber_).append(": (").append(y).append(',').append(x).endl();
            if (x == -1 || y == -1) {
                return false;
            }
            last_[player_] = board_;

            if (first_) {
                ++gameCount_;
                output.append("Game #").append(gameCount_).endl();
                first_ = false;
            }
            if (x == 0 || y == 0) {
                onPass(Player,0,0);
                return true;
            }
            
            if (board_[y-1][x-1].color != None) {
                onPass(Invalid,y,x);
                return true;
            }
            
            bool p = put(y-1,x-1);
            lastPassed_ = p;
            if (!lastPassed_) {
                auto result = seen_boards_.insert(board_);
                if (!result.second) {
                    onEnd(true);
                }
            }
            printBoard();
            output.endl();
            return true;
        }

    private:
        static Color other(Color p) {
            assert(p != None);
            return p == Black? White : Black;
        }

        bool put(int y, int x) {
            board_[y][x].color = player_;
            int killed[2] {floodFill(y,x,player_),0};
            Color c;

            if (y > 0 && (c=board_[y-1][x].color) != None) {
                killed[c!=player_] += floodFill(y-1,x,c);
            }
            if (y < BOARD_SIZE && (c=board_[y+1][x].color) != None) {
                killed[c!=player_] += floodFill(y+1,x,c);
            }
            if (x > 0 && (c=board_[y][x-1].color) != None) {
                killed[c!=player_] += floodFill(y,x-1,c);
            }
            if (x < BOARD_SIZE && (c=board_[y][x+1].color) != None) {
                killed[c!=player_] += floodFill(y,x+1,c);
            }
            
            if (killed[0]>0 && !killed[1]) {
                unvisit();
                board_[y][x].color = None;
                onPass(Taboo,y+1,x+1);
                return true;
            }
            else if (killed[1]>0) {
                Color enemy = other(player_);
                killall(enemy);
                unvisit();
                if (board_ == last_[enemy]) {
                    onPass(Taboo,y+1,x+1);
                    board_ = last_[player_];
                    return true;
                }
            }
            else {
                unvisit();
            }
            return false;
        }
        
        void killall(Color c) {
            for (int j=0;j<BOARD_SIZE;++j) {
                for (int i=0;i<BOARD_SIZE;++i) {
                    auto &b = board_[j][i];
                    if (b.visited && b.color == c) {
                        b.color = None;
                    }
                }
            }
        }

        void unvisit() {
            for (int j=0;j<BOARD_SIZE;++j) {
                for (int i=0;i<BOARD_SIZE;++i) {
                    auto &b = board_[j][i];
                    if (b.visited) {
                        b.visited = false;
                    }
                }
            }
        }



        int floodFill(int sy, int sx, Color value) {
            auto saved = board_;
            std::queue<ii> q;
            q.push(std::make_pair(sy,sx));
            int count = 0,x,y;
            do {
                std::tie(y,x) = q.front();
                q.pop();
                if (y>=0 && y<BOARD_SIZE && x>=0 && x<BOARD_SIZE) {
                    auto& b = board_[y][x];
                    if (!b.visited) {
                        if (b.color == value) {
                            ++ count;
                            b.visited = true;
                            q.push(std::make_pair(y+1,x));
                            q.push(std::make_pair(y-1,x));
                            q.push(std::make_pair(y,x+1));
                            q.push(std::make_pair(y,x-1));
                        }
                        else if(b.color == None) {
                            board_ = saved;
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
                  .append(player_==White?"white":"black")
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
            std::array<char,BOARD_SIZE+1> line;
            line[BOARD_SIZE] = '\n';
            for (int j=0;j<BOARD_SIZE;++j) {
                for (int i=0;i<BOARD_SIZE;++i) {
                    line[i] = "+eo"[board_[j][i].color];
                }
                output.append(line.data(),line.size());
            }
        }

        void onEnd(bool tie) {
            output.append("Total ").append(turnNumber_).append(" turn(s)\n");
            if (tie) 
                output.append("(Tie)\n");
            printBoard();    
            reset();
        }

        void reset() {
            for (auto& row : board_) {
                row.fill(Intersection());
            }

            player_ = White;
            first_ = true;
            turnNumber_ = 0;
            lastPassed_ = false;
            seen_boards_.clear();
        }

        struct Intersection {
            Intersection() : color(None), visited(false) {}
            Color color;
            bool visited;

            bool operator==(const Intersection& rhs) const {
                return color == rhs.color;
            }
        };

        bool first_ = true;
        using BoardType = std::array<std::array<Intersection,BOARD_SIZE>,BOARD_SIZE>;
        BoardType board_,
                  last_[3];
        Color player_ = White;
        int gameCount_ = 0;
        int turnNumber_ = 0;
        bool lastPassed_ = false;
        std::unordered_set<BoardType> seen_boards_;
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

