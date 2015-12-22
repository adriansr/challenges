#include "../../helper/helper.1.inc"
#include <cstdlib>

std::vector<bool> forbidden_cells;
std::vector<bool> diagonal_taken[2];
std::vector<bool> row_taken;

int N;

bool inline place(int col, int row) {
    return ! ( row_taken[row] 
            || diagonal_taken[0][col-row+N]
            || diagonal_taken[1][row+col]
            || forbidden_cells[row*N + col] );
}

int backtrack(int col) {

    int count = 0;

    for (int row = 0; row < N; ++row) {
        if ( place(col,row) ) {
            int d1 = col - row + N,
                d2 = row + col;

            row_taken[row] = 
                diagonal_taken[0][d1] = 
                    diagonal_taken[1][d2] = true;
            
            int next = col + 1;      
            count += next<N? backtrack(next) : 1;

            row_taken[row] = 
                diagonal_taken[0][d1] = 
                    diagonal_taken[1][d2] = false;
        }
    }

    return count;
}

int main() {
	
    std::string line;

    for (int test_count = 1;std::getline(std::cin,line).good() && !line.empty();++test_count) {
        
        N = std::stoi(line,nullptr,10);
        if (N == 0)
            break;
        
        forbidden_cells.clear();
        forbidden_cells.resize(N*N);
        for (int row = 0, pos = 0; row < N; ++row, pos+=N) {
            std::getline(std::cin,line);
            for (int col = 0; col < N; ++col) {
                if (line[col] == '*') {
                    forbidden_cells[pos + col] = true;
                }
            }
        }
        
        row_taken.clear();
        row_taken.resize(N);
        diagonal_taken[0].clear();
        diagonal_taken[0].resize(2*N);
        diagonal_taken[1].clear();
        diagonal_taken[1].resize(2*N);

        const int count = backtrack(0);

        std::cout << "Case " << test_count << ": " << count << "\n";
    }
}

