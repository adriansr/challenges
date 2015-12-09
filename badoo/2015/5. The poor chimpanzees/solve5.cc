/* solve5.cc / The Poor Chimpanzees Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * This implements the following sequence:
 * https://oeis.org/A000975
 *
 * How to build
 * clang++ -std=c++11 solve5.cc -o solve
 * or
 * g++ -std=c++11 solve5.cc -o solve
 */

#include <string>
#include <iostream>

std::string bignum_to_str(__uint128_t val) {

    std::string s;
    do {
        s.push_back( (val % 10) + '0');
        val /= 10;
    }
    while (val);
    std::reverse(s.begin(),s.end());
    return s;
}

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        std::getline(std::cin,line);
        const int N = std::stoi(line,nullptr,10);
        
        __uint128_t val = 0;
        for (int i = 0 ; i <= N ; ++i) {
            val <<= 1;
            if (i&1) {
                val |= 1;
            }
        }
        std::cout << bignum_to_str(val) << "\n";

    }
}

