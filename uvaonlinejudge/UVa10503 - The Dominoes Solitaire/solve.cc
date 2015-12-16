#include "../../helper/helper.1.h"
#include <set>
#include <utility>

using Piece = std::pair<char,char>;
using PieceBag = std::vector<Piece>;

Piece read_piece(const std::string& line) {
    return std::make_pair(line[0],line[2]);
}

bool solve(char target,
           int num_pending,
           const PieceBag& bag,
           int used_mask,
           char final_target) {
    if (num_pending == 0)
        return target == final_target;
 
    for (int i=0;i<bag.size();++i) {
        const Piece& piece = bag[i];
        bool is_first;
        if (! (used_mask&(1<<i))
         && ((is_first=piece.first==target) || piece.second==target) ) {
            if (solve(is_first? piece.second : piece.first,
                      num_pending - 1,
                      bag,
                      used_mask | (1<<i),
                      final_target)) {
                return true;
            }

        }
    }
    return false;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    for (int test_num = 0; reader.get_line(line) ; ++test_num) {

        const int N = helper::string_to_int(line);
        if (N == 0) {
            break;
        }
        
        reader.get_line(line);
        const int M = helper::string_to_int(line);
        
        reader.get_line(line);
        const Piece left = read_piece(line);

        reader.get_line(line);
        const Piece right = read_piece(line);
        
        PieceBag bag;
        for (int i=0; i<M; ++i) {
            reader.get_line(line);
            const auto piece = read_piece(line);
            bag.push_back( piece );
        }
        
        output.append(solve(left.second,N,
                      bag,0,right.first) ? "YES\n" : "NO\n" );
    }
}

