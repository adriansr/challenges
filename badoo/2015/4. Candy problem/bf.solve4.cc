/* solve4.cc / Candy Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve4.cc -o solve
 * or
 * g++ -std=c++11 solve4.cc -o solve
 */

#include "helper.1.h"
#include <cassert>

#define DEBUG( X ) X


typedef int Value;
typedef int Count;
typedef std::vector<int> Vi;
typedef std::vector< Vi > VVi;

class CandyProblem {
    public:
        CandyProblem(const CandyProblem& p) = default;
        CandyProblem& operator=(const CandyProblem& p) = default;

        CandyProblem(int num_coins, int num_candy,
                     const std::vector<Value>& coin_values,
                     const std::vector<Count>& coin_counts,
                     const std::vector<Value>& candy_values,
                     const std::vector<Count>& candy_counts)
            : ncoins_(num_coins),
              ncandy_(num_candy) {

            assert( coin_values.size() == ncoins_);
            assert( coin_counts.size() == ncoins_);
            assert( candy_values.size() == ncandy_);
            assert( candy_counts.size() == ncandy_);

            vcoins_.reserve(num_coins);
            for (int i = 0 ; i<num_coins; ++i) {
                vcoins_.push_back( Coin(coin_values[i],coin_counts[i]) );
            }

            vcandy_.reserve(num_candy);
            for (int i = 0 ; i<num_candy; ++i) {
                vcandy_.push_back( Candy(candy_values[i],candy_counts[i]) );
            }


            // make sure coins are sorted from smaller to larger!
            std::sort(vcoins_.begin(),vcoins_.end(),
                    [](const Coin&a, const Coin&b) { return a.value < b.value;});

            /*for (int i=0;i<ncoins_;++i) {
                std::cerr << "(" << vcoins_[i].value 
                              << ")|" << vcoins_[i].count << "|\n";
            }

            int cost = 100;
            VVi rv = choices_for_cost(cost);
            for (const auto& r : rv) {
                std::cerr << "cost[" << cost << "] : ";
                for (int i=0;i<ncoins_;++i) {
                    if (r[i]) {
                        std::cerr << r[i] << "x(" << vcoins_[i].value 
                                  << ")|" << vcoins_[i].count << "| ";
                    }
                }
                std::cerr << "\n";
            }*/
        }
        
        int maximize() const {

            int rv = 0,
                money = total_money();
            
            for (int candy_id = 0; candy_id < ncandy_ ; ++candy_id) {
                
                const auto& candy = vcandy_[candy_id];

                if (money >= candy.value) {
                    const auto choices = choices_for_cost( candy.value );

                    for (auto& coins_used : choices) {

                        // be greedy: buy as much as possible
                        int count = 1;
                        if (candy.count > count)
                            count = maximize_purchase(candy.count,coins_used);

                        CandyProblem p(*this);

                        p.buy(candy_id, count, coins_used);
                        int candidate = count + p.maximize();
                        if (candidate > rv) {
                            rv = candidate;
                            //std::cerr << "c: " << rv << "\n";
                        }
                    }
                }
            }

            return rv;
        }
    protected:

        void buy(int candy_id, int count, const Vi& used_coins) {
            vcandy_[candy_id].count -= count;
            for (int i=0;i<ncoins_;++i) {
                vcoins_[i].count -= used_coins[i] * count;
            }
        }

    private:
        int maximize_purchase(int available_candies, const Vi& used_coins) const {
            int max = available_candies;
            for (int i = 0; i < ncoins_ ; ++i) 
                if (used_coins[i]) {
                    int candidate = vcoins_[i].count / used_coins[i];
                    if (candidate < max) {
                        max = candidate;
                    }
                }
            return max;
        }

        int total_money() const {
            int total = 0;
            for (const auto& coin : vcoins_) {
                total += coin.value + coin.count;
            }
            return total;
        }

        VVi choices_for_cost(int cost) const {

            VVi retval;
            Vi state;

            state.resize(ncoins_);

            choices_for_cost_it(cost,state,ncoins_-1,retval);

            return retval;
        }
        
        void choices_for_cost_it(int cost, Vi state, int coin_id, VVi& retval) const {

            const Coin this_coin = vcoins_[coin_id];

            for (int num = 0,lim=this_coin.count; num <= lim; ++num) {
                int val = num * this_coin.value;
                state[coin_id] = num;
                if (val >= cost) {
                    retval.push_back(state);
                    break;
                }
                else {
                    if (coin_id > 0)
                        choices_for_cost_it(cost - val, state, coin_id-1, retval);
                }
            }

        }

        int ncoins_,
            ncandy_;

        struct Coin {
            Coin(Value v, Count c) : value(v), count(c) {}

            Value value;
            Count count;
        };

        struct Candy {
            Candy(Value v, Count c) : value(v), count(c) {}

            Value value;
            Count count;
        };

        std::vector<Coin> vcoins_;
        std::vector<Candy> vcandy_;
};

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        std::getline(std::cin,line);
        const auto params = helper::num_array<int,2>(line);

        const int NUM_COINS = params[0],
                  NUM_CANDY = params[1];
        
        std::getline(std::cin,line);
        const auto coin_values = helper::num_vector<int>(line);
        std::getline(std::cin,line);
        const auto coin_counts = helper::num_vector<int>(line);
        std::getline(std::cin,line);
        const auto candy_values = helper::num_vector<int>(line);
        std::getline(std::cin,line);
        const auto candy_counts = helper::num_vector<int>(line);

        CandyProblem p(NUM_COINS,NUM_CANDY,coin_values,coin_counts,candy_values,candy_counts);
        
        std::cout << p.maximize() << "\n";       
    }
}

