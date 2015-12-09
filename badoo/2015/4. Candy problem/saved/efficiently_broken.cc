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
#include <sstream>
#include <cassert>

#define DEBUG( X ) X
#define INF 1000000000

typedef int Value;
typedef int Count;
typedef std::vector<int> Vi;
typedef std::vector< Vi > VVi;

#define LOG( X ) 

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
            }*/
            
            /*int cost = 8;
            auto rv = choices_for_cost(cost);
            for (const auto& r : rv.first) {
                const auto& r = rv.first;
                std::cerr << "cost[" << cost << "] : ";
                for (int i=0;i<ncoins_;++i) {
                    if (r[i]) {
                        std::cerr << r[i] << "x(" << vcoins_[i].value 
                                  << ")|" << vcoins_[i].count << "| ";
                    }
                }
                std::cerr << " -- " << rv.second << "\n";
            //}*/
        }
        
        

        int maximize() {

            int rv = 0,
                money = total_money();
            LOG( std::string log; )

            //ChoiceResult best;
            //best.second = INF;
            //int selected_candy = -1;
            struct Purchase {
                int candy_id;
                //int count;
                VVi coin_combinations;
            };
            std::vector<Purchase> candidates;
            int best_cost  = INF;
            int best_count = 0;

            for (int candy_id = 0; candy_id < ncandy_ ; ++candy_id) {
                
                const auto& candy = vcandy_[candy_id];
                
                if (candy.count>0 && money >= candy.value) {
                    Purchase p;
                    p.candy_id = candy_id;
                    ChoiceResult choice = choices_for_cost( candy.value );

                    int cost = choice.second;
                    if (cost < INF) {
                        if (candidates.empty() || cost < best_cost) {
                            candidates.clear();
                            ChoiceResult filtered = filter(choice.first,
                                                        [this,&candy](const Vi& item) {
                                                            return maximize_purchase(candy.count,item);
                                                        });
                            p.coin_combinations = filtered.first;
                            //p.count = filtered.second;
                            candidates.push_back( p );
                            best_cost = cost;
                            best_count = filtered.second;
                        }
                        else if (best_cost == cost) {
                            ChoiceResult filtered = filter(choice.first,
                                                        [this,&candy](const Vi& item) {
                                                            return maximize_purchase(candy.count,item);
                                                        });
                            if (filtered.second >= best_count) {
                                p.coin_combinations = filtered.first;
                                if (filtered.second>best_count) {
                                    candidates.clear();
                                    best_count = filtered.second;
                                }
                                candidates.push_back( p );
                            }
                        }
                    }
                }
            }
            
            for ( const auto& purchase : candidates) {

                for (const auto& coins_used : purchase.coin_combinations) {

                    // be greedy: buy as much as possible
                    int count = best_count;

                    CandyProblem p(*this);

                    p.buy(purchase.candy_id, count, coins_used);
                    int val = count + p.maximize();
                    if (val > rv) {
                        rv = val;
                        LOG ( log = p.log(); )
                        //std::cerr << "c: " << rv << "\n";
                    }
                }
            }
        
            LOG ( log_.append(log); )
            return rv;
        }

        LOG ( const std::string& log() const { return log_; } ) 

    protected:

        void buy(int candy_id, int count, const Vi& used_coins) {
            vcandy_[candy_id].count -= count;
            for (int i=0;i<ncoins_;++i) {
                vcoins_[i].count -= used_coins[i] * count;
            }
            
            LOG( std::stringstream ss;

            ss << "bought " << count << " candy " << candy_id << ":" << vcandy_[candy_id].value << " using";
            for (int i=0;i<ncoins_;++i)
                if (used_coins[i]) {
                    ss << " (" << vcoins_[i].value << ")x" << count * used_coins[i];
                }
            ss << "\n";

            log_ = ss.str(); )
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
        
        using ChoiceResult = std::pair<VVi,int>;

        static ChoiceResult filter(const VVi& input, std::function<int(const Vi&)> pred) {
            ChoiceResult output;
            output.second = INF;
            for (const auto &item : input) {
                int val = pred(item);
                if (val != INF) {
                    if (val < output.second) {
                        output.first.clear();
                        output.second = val;
                    }
                    if (val <= output.second) {
                        output.first.push_back(item);
                    }
                }
            }
            return output;
        }

        ChoiceResult choices_for_cost(int cost) const {

            Vi state;
            state.resize(ncoins_);

            VVi retval;

            int best = choices_for_cost_it(cost,state,0,retval,INF);
            //for (const auto& r : rv.first) {
            /*    std::cerr << "cost[" << cost << "] : ";
                for (int i=0;i<ncoins_;++i) {
                    if (retval[i]) {
                        std::cerr << retval[i] << "x(" << vcoins_[i].value 
                                  << ")|" << vcoins_[i].count << "| ";
                    }
                }
                std::cerr << " -- " << best << "\n";*/

            return ChoiceResult(retval,best);
        }
        
        int choices_for_cost_it(int cost, Vi state, int coin_id, VVi& retval,int best) const {

            const Coin this_coin = vcoins_[coin_id];

            for (int num = 0,lim=this_coin.count; num <= lim; ++num) {
                int total = num * this_coin.value;
                state[coin_id] = num;
                if (total > cost) {
                    if (total < best) {
                        best = total;
                        retval.clear();
                        retval.push_back( state );
                    }
                    else if (total == best) {
                        retval.push_back(state);
                    }
                    break;
                }
                else if (coin_id + 1 < ncoins_) {
                    best = total + choices_for_cost_it(cost - total, state, coin_id+1, retval, best);
                }
            }

            return best;
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

        LOG( std::string log_; )
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
        LOG ( std::cerr << p.log() << "\n"; )
    }
}

