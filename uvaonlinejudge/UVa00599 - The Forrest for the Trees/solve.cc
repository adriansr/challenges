#include "../../helper/helper.1.h"

helper::LineReader<> reader;
std::string line;

#define N 26

struct {
    uint_fast32_t g[N];
    uint_fast32_t seen[N];
    //bool  colored[N];
    uint_fast32_t colored;
    int num_neighbours[N];
} state, clear;

int pending[N],
    pending_index;

int main() {
	
	std::string output;
	output.reserve(4096);

    reader.get_line(line);
    const int num_tests = helper::string_to_int(line);

    for (int test_num = 1; test_num <= num_tests; ++test_num) {
        if (test_num>1) {
            state = clear;
        }

        while (reader.get_line(line) && line[0]!='*') {
            int a = line[1] - 'A',
                b = line[3] - 'A';

            state.g[a] |= 1 << b;
            state.g[b] |= 1 << a;
            ++ state.num_neighbours[a];
            ++ state.num_neighbours[b];
        }

        int ntrees   = 0,
            naccorns = 0;

        reader.get_line(line);
        for (int idx=0,len=line.size();idx<len;idx+=2) {
            int node = line[idx] - 'A';
            if (state.num_neighbours[node]>0) {
                if (!(state.colored&(1<<node))) {
                    ++ntrees;

                    state.colored |= 1 << node;

                    for (pending_index=1,pending[0]=node;pending_index>0;) {
                        node = pending[--pending_index];
                        uint_fast32_t gnode = state.g[node];

                        for (int j=0,remain=state.num_neighbours[node];remain>0;++j) {
                            uint_fast32_t mask = 1 << j;
                            if ( (gnode & mask ) ) {
                                -- remain;
                                if( !(state.colored&mask ) ) {
                                    state.colored |= mask;
                                    pending[pending_index++] = j;
                                }
                            }
                        }
                    }
                }
            }
            else {
                ++ naccorns;
            }
        }

        output.append("There are ")
              .append(std::to_string(ntrees))
              .append(" tree(s) and ")
              .append(std::to_string(naccorns))
              .append(" acorn(s).\n");
    }

	write(1,output.data(),output.size());
}

