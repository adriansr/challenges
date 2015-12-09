/* solve5.cc / Cheating Classes Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve5.cc -o solve
 * or
 * g++ -std=c++11 solve5.cc -o solve
 */
#include "helper.1.h"

#include <string>
#include <iostream>
#include <map>

// analyzes all exams and returns true if at least half of them share
// a pattern of 'run_length' size
int pattern_detect( const std::vector<std::string>& exams, 
                    int num_questions, int num_answers, int run_length) {

    int trigger = exams.size() / 2;
    if (exams.size()&1) {
        ++trigger;
    }
    
    for (size_t pos = 0, limit = num_questions - run_length; pos <= limit; ++pos) {
        std::map<std::string,int> matches;
        for ( const auto& exam : exams) {
            if ( ++ matches[ exam.substr(pos,run_length) ] >= trigger ) {
                return true;
            }
        }
    }
    return false;
}

void fix_windows_crlf(std::string& s) {
    size_t len = s.size();
    if (s[len-1] == '\r') {
        s.resize(len-1);
    }
}

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);
    
    for (int test_count = 0 ; test_count < T ; ++test_count ) {
        
        std::cerr << "#" << test_count << "\n";
        std::getline(std::cin,line);

        const auto params = helper::num_array<int,4>(line);
        const int C = params[0],
                  Q = params[1],
                  A = params[2],
                  S = params[3];
        std::vector<int> cheaters;
        
        for (int class_count=0;class_count<C;++class_count) {

            std::getline(std::cin,line);
            fix_windows_crlf(line);

            size_t skip = line.find(' ');

            std::vector<std::string> exams;

            helper::for_each_word<>(line,[&exams](const std::string& word) {
                                                exams.push_back(word);
                                            },skip+1);
            
            if (pattern_detect(exams,Q,A,S) ) {
                cheaters.push_back(class_count);
            }
        }
        
        if (cheaters.size()>0) {
            std::cout << cheaters[0];
            for (int i=1;i<cheaters.size();++i) {
                std::cout << " " << cheaters[i];
            }
        }
        std::cout << "\n";
    }
}

