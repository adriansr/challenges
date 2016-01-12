#include "../../helper/helper.1.h"
#include <queue>
#include <cassert>

#define DEBUG(X) 
//#define DEBUG(X) std::cerr << X

constexpr int NUM_PIECES = 9,
              SIDE_SIZE = 3;

std::array<std::array<char,NUM_PIECES>,NUM_PIECES> manhattan_distance;

struct Puzzle {
    
    using Board = std::array<char,NUM_PIECES>;

    Puzzle(const std::string& line) {
        for (int position=0;position<NUM_PIECES;++position) {
            char val = line[position<<1];
            if (val <= '9') {
                board[position] = val-'0';
            }
            else {
                board[position] = 9;
                empty_pos = position;
            }
        }
    }

    Board board;
    int empty_pos;
    std::string moves;

    int cost() const {
        return moves.size();
    }

    int heuristic_cost() const {
        int sum_distances = 0;
        for (int pos=0;pos<NUM_PIECES;++pos) {
            int piece = board[pos];
            if (piece != 9)
                sum_distances += manhattan_distance[piece][pos];
        }
        return sum_distances;
    }

    bool is_unsolvable() const {
        int nfails = 0;
        if (board[NUM_PIECES-1] == 9) {
            for (int pos=0;pos<NUM_PIECES-1;++pos) {
                if (board[pos] != pos+1)
                    ++ nfails;
            }
        }
        return nfails == 2;
    }

    bool can_slide_up() const {
        return empty_pos>=SIDE_SIZE;
    }

    bool can_slide_down() const {
        return empty_pos+SIDE_SIZE<NUM_PIECES; 
    }

    bool can_slide_left() const {
        return (empty_pos%SIDE_SIZE)>0;
    }

    bool can_slide_right() const {
        return (empty_pos%SIDE_SIZE)<SIDE_SIZE-1;
    }

    bool slide(char move) {
        bool ok;
        switch(move) {
            case 'u':
                if ( (ok=can_slide_up()) )
                    do_swap(empty_pos-SIDE_SIZE);
                break;
            case 'd':
                if ( (ok=can_slide_down()) )
                    do_swap(empty_pos+SIDE_SIZE);
                break;
            case 'l':
                if ( (ok=can_slide_left()) )
                    do_swap(empty_pos-1);
                break;
            case 'r':
                if ( (ok=can_slide_right()) )
                    do_swap(empty_pos+1);
                break;
            default:
                ok = false;
        }
        return ok;
    }

    void do_swap(int new_pos) {
        assert(board[empty_pos]==9);
        assert(new_pos != empty_pos);
        std::swap(board[new_pos],board[empty_pos]);
        empty_pos = new_pos;
    }

    struct Iterator {
        Iterator(bool u, bool d, bool l, bool r) : flags(0) {
            if (u)
                flags = 0x1;
            if (d)
                flags = (flags<<3) | 0x2;
            if (l)
                flags = (flags<<3) | 0x3;
            if (r)
                flags = (flags<<3) | 0x4;
        }

        Iterator() : flags(0) {
        }
        
        bool operator!=(const Iterator& other) const {
            return flags != other.flags;
        }

        void operator++() {
            flags >>= 3;
        }

        char operator*() const {
            static const char *flag_val = "xudlr";
            return flag_val[ flags & 7 ];
        }

        int flags;
    };

    Iterator possible_moves() const {
        return Iterator(can_slide_up(),
                        can_slide_down(),
                        can_slide_left(),
                        can_slide_right());
    }

    void debug() const {
        for (int y=0,pos=0;y<SIDE_SIZE;++y) {
            for (int x=0;x<SIDE_SIZE;++x,++pos) {
                DEBUG(' ' << static_cast<char>('0'+board[pos]));
            }
            DEBUG('\n');
        }
        DEBUG('\n');
    }

};

struct State {
    // state from base puzzle
    State(const Puzzle& base) 
    : p(base) {
        // at end
        calc_costs();
    }

    // state from puzzle aplying a move
    State(const State& base, char move) 
    : p(base.p)
    , moves(base.moves) {
        bool move_applied = slide(move);
        assert(move_applied);
        // at end
        calc_costs();
    }
    
    Puzzle p;
    int minus_cost,
        distance_to_solution;
    std::string moves;

    void debug() const {
        DEBUG("Cost:  " << minus_cost << "(" << distance_to_solution << ")\n"
                  << "Moves: " << moves << "\n"); 
        p.debug();
    }

    bool operator<(const State& other) const {
        return minus_cost < other.minus_cost;
    }

    void calc_costs() {
        distance_to_solution = p.heuristic_cost();
        minus_cost = - (moves.size() + distance_to_solution);
    }

    bool is_solution() const {
        return distance_to_solution == 0;
    }

    bool slide(char move) {
        moves.push_back(move);
        return p.slide(move);
    }

    Puzzle::Iterator begin() const {
        return p.possible_moves();
    }

    Puzzle::Iterator end() const {
        return Puzzle::Iterator();
    }
};


void precalc_manhattan_distances() {
    
    for (int piece=1;piece<NUM_PIECES; ++piece) {
        int target_x = (piece-1) % SIDE_SIZE,
            target_y = (piece-1) / SIDE_SIZE;

        for (int y=0,pos=0;y<SIDE_SIZE;++y) {
            int dist_y = y<target_y? target_y-y : y-target_y;
            for (int x=0;x<SIDE_SIZE;++x,++pos) {
                int dist_x = x<target_x? target_x-x : x-target_x;
                manhattan_distance[piece][pos] = dist_x + dist_y;
            }
        }
    }
}

std::string solve(const Puzzle& base_case) {
    
    base_case.debug();

    std::priority_queue<State> prioq;
    std::set<Puzzle::Board> visited;

    prioq.emplace(base_case);

    do {
        const State state = prioq.top();
        prioq.pop();
        
        state.debug();

        if (state.is_solution()) {
            DEBUG("Found solution!\n");
            return state.moves;
        }

        if (state.p.is_unsolvable()) {
            DEBUG("Found unsolvable!\n");
            break;
        }
        
        for (auto move : state) {
            State next(state,move);
            if (visited.find(next.p.board) == visited.cend()) {
                visited.insert(next.p.board);
                prioq.push(next);
            }
        }
    }
    while (!prioq.empty());

    return "unsolvable";
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    precalc_manhattan_distances();

    const int num_cases = helper::string_to_int(reader());

    for (int ncase=0;ncase<num_cases;++ncase) {

        reader();
        reader.get_line(line);
        
        if (ncase) {
            output.endl();
        }

        std::string solution = solve(line);
        output.append(solution).endl();
    }
}

