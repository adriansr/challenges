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
#include <future>

// tells if a single exam matches a pattern 
int exam_matches(const std::string& exam, const std::string& pattern,
                 size_t start, size_t end) {
    for (int i = start ; i < end ; ++i) {
        if (exam[i] != pattern[i])
            return false;
    }
    return true;
}

// returns true if the current pattern matches the number of required exams,
// otherwise it shrinks the pattern and calls itself recursively
bool find_matching_pattern(const std::vector<std::string>& exams,
                          const std::string& pattern,
                          size_t pattern_start,
                          size_t pattern_end,
                          int run_length,
                          int trigger) {
    
    for (int matches = 0, i = 0; i < exams.size() ; ++i ) {
        if (exam_matches(exams[i],pattern,pattern_start,pattern_end)) {
            if (++ matches >= trigger) {
                return true;
            }
        }
    }

    if (pattern_end - pattern_start > run_length) {
        if ( find_matching_pattern(exams,pattern,pattern_start+1,pattern_end,run_length,trigger) 
          || find_matching_pattern(exams,pattern,pattern_start,pattern_end-1,run_length,trigger)) {
            return true;
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

        if (end-start>=run_length) {
            if( find_matching_pattern(exams,pattern,start,end,run_length,trigger) )
                return true;
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

using ExamList = std::vector<std::string>;
struct TestCase {
    int C,Q,A,S;
    std::vector<ExamList> classes;

    std::future<std::string> result;
};

std::string run_test(TestCase *ptr) {
    std::vector<int> cheaters;
    for (int class_num = 0; class_num < ptr->C; ++class_num) {
        if (pattern_detect(ptr->classes[class_num],ptr->Q,ptr->A,ptr->S) ) {
            cheaters.push_back(class_num);
        }
    }

    std::string r;
    if (cheaters.size()>0) {
        r.append(std::to_string(cheaters[0]));
        for (int i=1;i<cheaters.size();++i) {
            r.push_back(' ');
            r.append(std::to_string(cheaters[i]));
        }
    }
    r.push_back('\n');
    return r;
}


#define NTHREADS 8

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);
    
    TestCase cases[NTHREADS];
    int pos = 0;

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        std::getline(std::cin,line);

        const auto params = helper::num_array<int,4>(line);

        TestCase* tcase;
        if (pos == NTHREADS) {
            for (int i=0;i<NTHREADS;++i) {
                auto res = cases[i].result.get();
                std::cout << res ;
            }
            pos = 0;
        }

        tcase = &cases[pos++];

        tcase->C = params[0],
        tcase->Q = params[1],
        tcase->A = params[2],
        tcase->S = params[3];
        
        tcase->classes.resize(tcase->C);

        for (int class_count=0;class_count<tcase->C;++class_count) {

            std::getline(std::cin,line);
            fix_windows_crlf(line);
            
            const int num_exams = std::stoi(line);

            // i just ignore the first number
            size_t skip = line.find(' ');
            
            ExamList& exams = tcase->classes[class_count];
            exams.clear();
            exams.reserve(num_exams);

            helper::for_each_word<>(line,[&exams](std::string&& word) {
                                                exams.push_back(word);
                                            },skip+1);
        }
        
        std::cerr << "# " << test_count << " ...\n";
        tcase->result = std::async(std::launch::async,run_test,tcase);
    }
    for (int i=0;i<pos;++i) {
        auto res = cases[i].result.get();
        std::cout << res ;
    }
}

