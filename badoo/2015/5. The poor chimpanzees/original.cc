/* solve5.cc / The Poor Chimpanzees Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve5.cc -o solve
 * or
 * g++ -std=c++11 solve5.cc -o solve
 */

#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <stdint.h>

#if 0
constexpr int INF = 10000000; //INT_MAX;
// for performance left and right are reversed
class Switches {
    public:
        
        Switches(int N)  {
            v_.resize(N,true);
        }
        
        int solve() const {
            Visited visited;
            return solve_(visited);
        }
        
    private:
        using Visited = std::set< std::vector<bool> >;
        
        bool visited(Visited&v) const {
            return v.find(v_) != v.end();
        }
        
        bool solved() const {
            const auto last = v_.end();
            return std::find(v_.begin(),last,true) == last;
        }

        int solve_(Visited& v) const {
            if (solved())
                return 0;
            
            /*for (bool val : v_) {
                std::cerr << (val? 'X':'_');
            }
            std::cerr << "\n";*/

            v.insert(v_);
            int other_choice = choice(),
                retval = eval_toggle(v,0);

            if (other_choice) {
                retval = std::min(eval_toggle(v,other_choice),retval);
            }

            return retval;

        }
        
        int eval_toggle(Visited& v, int pos) const {
            Switches s(*this);
            s.toggle(pos);
           // int r;
            //return !s.visited(v) && (r=s.solve_(v))!=INF? 1 + r : INF;
            if (!s.visited(v)) {
                int r = s.solve_(v);
                return r != INF ? r + 1 : INF;
            }
            else
                return INF;
        }

        void toggle(int pos) {
            v_[pos] = ! v_[pos];
        }

        // there's only one toggle choice (at most) 
        // plus the first switch that always can be toggled
        int choice() const {
            for (int i = 0,end = v_.size()-1; i < end; ++i) {
                if (v_[i]) {
                    return i+1;
                }
            }
            return 0;
        }

        std::vector<bool> v_;
};
#endif

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
        for (int i = 0 ; i < N ; ++i) {
            val <<= 1;
            if (i&1) {
                val |= 1;
            }
        }
        std::cout << bignum_to_str(val) << "\n";

    }
}

