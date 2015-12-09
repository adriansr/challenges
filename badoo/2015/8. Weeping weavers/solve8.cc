/* solve8.cc / Weeping Weavers Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve8.cc -o solve
 * or
 * g++ -std=c++11 solve8.cc -o solve
 */

#include <string>
#include <iostream>
#include <queue>
#include "helper.1.h"


int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        std::getline(std::cin,line);
        const auto params = helper::num_array<int,3>(line);

        const int G = params[0], // amount of goods
                  W = params[1], // num of workers
                  B = params[2]; // budget
   }
}

