#include "../../helper/helper.1.h"
#include <bitset>
#include <queue>
#include <cassert>

// this can cause WA if the judge has some larger lines
constexpr size_t MAX_INPUT_LINE_LENGTH = 4096;
constexpr int LIMIT = 10000,
              START = 1000,
              INF = LIMIT + 1;

std::bitset<LIMIT> is_prime;

int modify(int number, int digit, int value) {
    int remainder = number % digit,
        current = number % (digit * 10) - remainder;
    return number - current + (value * digit);
}

int bfs(int source, int dest) {
    
    assert(is_prime[source]);
    assert(is_prime[dest]);
    Vi distance(LIMIT,INF);
    distance[source] = 0;

    std::queue<int> q;
    q.push(source);

    do {
        int current = q.front();
        q.pop();

        if (current == dest) {
            return distance[dest];
        }

        for (int digit = 1; digit<10000; digit*=10) {
            for (int value=0;value<=9;++value) {
                int next = modify(current,digit,value);
                if (next>START && is_prime[next] && distance[next]==INF) {
                    distance[next] = 1 + distance[current];
                    q.push(next);
                }
            }
        }
    }
    while(!q.empty());
    return -1;
}

void precalc_primes() {
    is_prime.set();
    is_prime[0] = is_prime[1] = false;
    for (int i=4;i<LIMIT;i+=2) {
        is_prime[i] = false;
    }
    for (int i=3;i<LIMIT;i+=2) {
        if (is_prime[i]) {
            for (int j=i+i;j<LIMIT;j+=i) {
                is_prime[j] = false;
            }
        }
    }
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<MAX_INPUT_LINE_LENGTH> reader; // <-- check or RE
    
    precalc_primes();

    const int NUM_TESTS = helper::string_to_int(reader());

    std::array<int,2> params;
    for (int test_num=0;test_num<NUM_TESTS;++test_num) {
        params = helper::num_array<int,2>(reader());
        int N = params[0],
            M = params[1];
        int result = bfs(N,M);
        if (result>=0)
            output.append(result).endl();
        else
            output.append("Impossible\n");
    }
}

