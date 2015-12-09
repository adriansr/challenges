/* solve7.cc / Seek and Destroy Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve7.cc -o solve
 * or
 * g++ -std=c++11 solve7.cc -o solve
 */

#include <string>
#include <iostream>
#include <queue>
#include "helper.1.h"

struct Problem {
    int max_steps;
    int width, height;
    int initial_mice;

    std::string area;

    char& at(int x, int y) {
        return area[ x + height*y ];
    }

    bool traversable(int x, int y) {
        return x>=0 && x < width
            && y>=0 && y < height
            && at(x,y) != 'X';
    }

    void draw(int x, int y);

    int start_x,
        start_y;
    
    int solve();
};

void Problem::draw(int xpos, int ypos) {
    std::cerr << "Pos (" << xpos << "," << ypos << " s:" << max_steps << " m:" << initial_mice 
        << "\n";
    for (int y = 0; y < height ; ++y) {
        for (int x = 0; x < width ; ++x) {
            if (x == xpos && y == ypos)
                std::cerr << '&';
            else
                std::cerr << at(x,y);
        }
        std::cerr << '\n';
    }
    std::cerr << '\n';
}

int Problem::solve() {
    
    if (initial_mice == 0) 
        return max_steps;

    struct Step {
        Step(int x, int y, int s, int mice) 
            : x(x), y(y), step(s), num_mice(mice) {}
        int x,y,step,num_mice;
    };

    std::queue<Step> queue;

    //draw(start_x,start_y);
    queue.push( Step(start_x,start_y,max_steps,initial_mice) );

    int best_result = -1;
    while(!queue.empty()) {
        auto s = queue.front(); queue.pop();
        char &cell = at(s.x,s.y);
        //std::cerr << "("<<s.x<<","<<s.y<<") " << cell << " step " << s.step << "\n";
        if (cell == 'V') continue;
        if ( cell == 'M') {
           // std::cerr << "Found mouse!";
            //std::cerr << "Mice found after " << s.step << " " << s.num_mice << "\n";
            if (s.num_mice - 1== 0) {
                best_result = std::max(best_result,s.step);
                continue;
            }
            else {
                //cell = 'V';
                Problem newp(*this);
                newp.initial_mice = s.num_mice - 1;
                newp.start_x = s.x;
                newp.start_y = s.y;
                newp.max_steps = s.step ;
                newp.area = area;
                std::replace(newp.area.begin(),newp.area.end(),'V','O');
                newp.at(s.x,s.y) = 'O';

                int r = newp.solve();
                //std::cerr << "subsearch returned " << r << "\n";
                best_result = std::max(r,best_result);
                continue;
            }
        }
        cell = 'V';
        if (--s.step>=0) {
            if (traversable(s.x-1,s.y)) {
                queue.push(Step(s.x-1,s.y,s.step,s.num_mice));
            }
            if (traversable(s.x+1,s.y)) {
                queue.push(Step(s.x+1,s.y,s.step,s.num_mice));
            }
            if (traversable(s.x,s.y-1)) {
                queue.push(Step(s.x,s.y-1,s.step,s.num_mice));
            }
            if (traversable(s.x,s.y+1)) {
                queue.push(Step(s.x,s.y+1,s.step,s.num_mice));
            }
        }
    }

    return best_result;
}

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        Problem problem;

        std::getline(std::cin,line);
        problem.max_steps = std::stoi(line,nullptr,10);
        
        std::getline(std::cin,line);
        const auto dims = helper::num_array<int,2>(line);
        
        problem.width  = dims[0];
        problem.height = dims[1];
        problem.area.reserve( problem.width * problem.height );
        problem.initial_mice = 0;

        for (int y = 0; y < problem.height; ++y) {
            std::getline(std::cin,line);
            for (int x = 0; x < problem.width; ++x) {
                char c = line[x*2];
                if (c == 'B') {
                    problem.start_x = x;
                    problem.start_y = y;
                    c = 'O';
                }
                else if (c == 'M') {
                    problem.initial_mice ++;
                }
                problem.area.push_back( c );
            }
        }
        //std::cerr << "Case# " << test_count << "\n";
        //std::cerr << dims[0] << ":" << dims[1] << " " << problem.max_steps << "\n";
        //std::cerr << "Area: " << problem.area << "\n";

        int steps_left = problem.solve();
        if (steps_left >= 0) {
            std::cout << steps_left << "\n";
        }
        else 
            std::cout << "NO\n";
    }
}

