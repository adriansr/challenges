/* solve8.cc / Weeping Weavers Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * WARNING:
 * This code requires the GNU Linear Programming Kit (glpk)
 * to be available, including development packages.
 *
 * https://www.gnu.org/software/glpk/
 *
 * In MacOS X I got it from 
 * $ brew install glpk
 *
 * For Linux systems it will be something like
 * # apt-get install libglpk libglpk-dev // or similar (untested)
 *
 * How to build
 * clang++ -std=c++11 solve8.cc -lglpk -o solve
 * or
 * g++ -std=c++11 solve8.cc -lglpk -o solve
 */

#include <string>
#include <iostream>
#include <queue>
#include <tuple>
#include <set>

#include "helper.1.h"
#include <glpk.h>

using Vi = std::vector<int>;

// this object layout might not make a lot of sense now, because they are
// the remains of an early attempt of solving the problem using DP

struct Worker {
    std::vector<int> wg; // hours it takes him to produce each good
    double time_left; // amount of hours he has left to work
};

struct Problem {
    static constexpr double IMPOSSIBLE = 100000000;

    int num_goods,
        num_workers;
    
    std::vector<Worker> worker;

    double solve() const;
};

double Problem::solve() const {

    /* This code sets up GLPK to solve the linear equations defined by the problem:
        
       our variables are
       xij , the number of hours a worker i works producing a good j

       there are two sets of equations:
    
       x11 * 1/Gw11 + x21 * 1/Gw21 + .. + xN1 * 1/GwN1 = 1
       x12 * 1/Gw12 + x22 * 1/Gw22 + .. + xN2 * 1/GwN2 = 1
        ...
       xM2 * 1/GwM2 + x22 * 1/Gw22 + .. + xN2 * 1/GwN2 = 1
       
       this means that all the work put together with the amount of hours
       it takes each worker to produce a specific good must be enough to
       produce the good ( = 1) 

       the second set of equations is:

       x11 + x12 + ... + x1N <= Wb1
       x21 + x22 + ... + x2N <= Wb2
       ...
       xM1 + xM2 + ... + xMN <= WbM

       this means that each worker can only work up to Wb hours in total.
       */

    glp_prob *lp = glp_create_prob();
    glp_set_obj_dir(lp,GLP_MIN);

    // this is the number of equations we have:
    const int num_equations = num_workers + num_goods;
    glp_add_rows(lp,num_workers + num_goods);
    
    // our first #num_goods rows have a fixed bound = 1
    // because we want to fully-produce every good
    for (int i = 0 ; i < num_goods; ++i) {
        glp_set_row_bnds(lp, i+1, GLP_FX, 1.0, 1.0);
    }

    // then the last #num_workers rows have a [0,Wbi] bound
    // because we don't want the workers work more than they
    // are capable to
    for (int i = 0; i < num_workers; ++i) {
        double work_limit = worker[i].time_left;
        glp_set_row_bnds(lp, num_goods + i + 1, GLP_DB, 0.0, work_limit );
    }

    // this is the number of unknown variables (xij) 
    // that's the amount of work a worker does for a particular good
    // from 1 (100% of work) to 0
    const int num_variables = num_goods * num_workers;
    glp_add_cols(lp, num_variables);
    for (int i = 0; i < num_variables;  ++i) {
        glp_set_col_bnds(lp,i+1,GLP_LO,0.0,0.0);
        // this is the coeficient for every unknown variable in our cost 
        // minimization fn . We weight all variables the same (1)
        glp_set_obj_coef(lp,i+1,1.0);
    }
    
    // let's fill the coeff matrix, which is quite large
    // but libglpk only cares about nonzero entries
    const size_t num_nonzero_cells = num_goods * num_workers * 2;
    std::vector<int32_t> ia, ja;
    std::vector<double> ar;
    ia.reserve(num_nonzero_cells+1);
    ja.reserve(num_nonzero_cells+1);
    ar.reserve(num_nonzero_cells+1);
    
    // this glpk library has some funny obsesion with 1-indexing
    ia.push_back(0);
    ja.push_back(0);
    ar.push_back(0);

    // fill coeffs for the first equations (goods)
    for (int worker_id = 0; worker_id < num_workers; ++worker_id) {
        for (int good_id = 0; good_id < num_goods; ++good_id) {
            // set entry for first set of equations (goods)
            double hours = worker[worker_id].wg[good_id];
            if (hours >= 1.0) {
                ia.push_back(good_id+1); // row 
                ja.push_back(worker_id*num_goods + good_id+1); // column 
                ar.push_back(1.0 / hours); // value
            }

            // set entry for second set of equations (work hours constrain)
            ia.push_back(num_goods + worker_id +1); // row 
            ja.push_back(worker_id*num_goods + good_id+1); // column 
            ar.push_back(1.0); // value
        }
    }
    // ar.size() - 1 because we don't count the first (0,0,0) element
    glp_load_matrix(lp,ar.size()-1,ia.data(),ja.data(),ar.data());

    // params for the simplex method
    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_OFF; // don't polute stdout pls
    
    // finally we're ready to let glpk do the magic
    int rv = glp_simplex(lp,&params);

    // if it fails or the result is not optimal, return IMPOSSIBLE
    if (rv != 0 || glp_get_status(lp)!=GLP_OPT) {
        return IMPOSSIBLE;
    }
    
    double z = glp_get_obj_val(lp);
    
    glp_delete_prob(lp);
    return z;
}

int main() {
    std::string line;

    std::getline(std::cin,line);
    const int T = std::stoi(line,nullptr,10);

    for (int test_count = 0 ; test_count < T ; ++test_count ) {

        Problem p;
        
        std::getline(std::cin,line);
        p.num_goods   = std::stoi(line,nullptr,10);

        std::getline(std::cin,line);
        p.num_workers = std::stoi(line,nullptr,10);

        std::getline(std::cin,line);
        const int budget = std::stoi(line,nullptr,10);
        
        p.worker.resize(p.num_workers);

        for (int w = 0; w < p.num_workers ; ++w) {
            std::getline(std::cin,line);
            const auto& desc = helper::num_vector<int>(line);
            p.worker[w].time_left = desc[0];
            p.worker[w].wg.insert(p.worker[w].wg.begin(),desc.begin()+1,desc.end());
        }

        double solution = p.solve();

        if (solution == Problem::IMPOSSIBLE) {
            std::cout << "NO\n";
        }
        else {
            double val = solution - budget;
            int rounded = val >= 0.0? static_cast<int>(solution - budget + 0.5)
                                    : static_cast<int>(solution - budget - 0.5);  
            if (rounded == 0) {
                std::cout << "OK\n";
            }
            else
                std::cout << rounded << "\n";
        }
   }
}

