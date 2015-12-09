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

// tells if a single exam matches a pattern 
int exam_matches(const std::string& exam, const std::string& pattern) {
    for (int i = 0 ; i < exam.size() ; ++i) {
        if (pattern[i]!=' ' && exam[i] != pattern[i])
            return false;
    }
    return true;
}

// returns true if the current pattern matches the number of required exams,
// otherwise it shrinks the pattern and calls itself recursively
bool find_matching_pattern(const std::vector<std::string>& exams,
                          const std::string& pattern,
                          int run_length,
                          int trigger,
                          int pattern_length) {
    
    if ( trigger <= std::count_if( exams.begin(), exams.end() ,
                                  [&pattern](const std::string& exam) 
                                    { return exam_matches(exam,pattern); })) {
        return true;
    }
    
    if (pattern_length > run_length) {
        size_t start = pattern.find_first_not_of(' '),
               end   = pattern.find_last_not_of(' ');

        for (int i = 0; i < 2; ++i ) {
            size_t pos = i? end : start;
            if (pos != pattern.npos) {
                auto new_pattern(pattern);
                new_pattern[pos] = ' ';
                if ( find_matching_pattern(exams,new_pattern,run_length,trigger,pattern_length-1) ) {
                        return true;
                }
            }
        }
    }

    return false;
}

// analyzes all exams and returns true if at least half of them share
// a pattern of 'run_length' size
int pattern_detect( const std::vector<std::string>& exams, 
                    int num_questions, int num_answers, int run_length) {

    int trigger = exams.size() / 2;
    if (exams.size()&1) {
        ++trigger;
    }
    
    std::string pattern;
    pattern.resize(num_questions,' ');
    
    for (int question = 0; question < num_questions; ++question) {
        std::vector<int> stats;
        stats.resize(num_answers);
        for (const auto& exam : exams) {
            if (++stats[ exam[question] - 'a' ] >= trigger ) {
                pattern[question] = exam[question];
                break;
            }
        }
    }
    
    // pattern might contain different runs of matches
    for (size_t pos = 0;pos<pattern.size();) {
        size_t start = pattern.find_first_not_of(' ',pos),
               end   = pattern.find(' ',start);
        pos = end;
        if (start == pattern.npos)
            break;

        if (end == pattern.npos) 
            end = pattern.size();

        std::string subpattern;
        subpattern.resize(pattern.size(),' ');

        for (size_t i = start; i < end; ++i)
            subpattern[i] = pattern[i];

        if (end-start>=run_length) {
            return find_matching_pattern(exams,subpattern,run_length,trigger,end-start);
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

        std::getline(std::cin,line);

        const auto params = helper::num_array<int,4>(line);
        const int C = params[0],
                  Q = params[1],
                  A = params[2],
                  S = params[3];
        std::vector<int> cheaters;
        
        std::cerr << test_count << "\n";
        for (int class_count=0;class_count<C;++class_count) {

            std::getline(std::cin,line);
            fix_windows_crlf(line);

            // i just ignore the first number
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

