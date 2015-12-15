#include "../../helper/helper.1.h"
#include <cstdlib>

#define FOR_LOOP(VAR,START,END) for(int VAR = START; VAR < END; ++VAR)

constexpr int INF_COST = 1000000,
              NUM_AREAS = 25,
              NUM_OFFICES = 5,
              GRID_SIDE = 5;

std::array<int,NUM_AREAS> population;
std::array<int,NUM_OFFICES> offices,
                            best_offices;

int best_cost;

struct UnusedDetector {
    static void visit_office(int office_id) {
        visited[office_id] = true;
    }
    static std::array<bool,NUM_OFFICES> visited;
};
std::array<bool,NUM_OFFICES> UnusedDetector::visited;

struct NOOP {
        static void visit_office(int){}
};

int pos_to_area_number(int x, int y) {
    return y * GRID_SIDE + x;
}

std::pair<int,int> area_number_to_pos(int num) {
    return std::make_pair(num / GRID_SIDE, num % GRID_SIDE);
}


template<class Visitor = NOOP,
         std::array<int,NUM_OFFICES>& state = offices>
int calculate_cost() {

    int total = 0;

    FOR_LOOP(area_id,0,NUM_AREAS) {

        int area_pop = population[area_id];
        if (area_pop) {

            struct {
                int cost = INF_COST,
                    office_id,
                    office_area;
            } best;
            
            auto area_pos = area_number_to_pos(area_id);

            FOR_LOOP(office_id,0,NUM_OFFICES) {
                int office_area = state[office_id];

                if (office_area != area_id) {
                    auto office_pos = area_number_to_pos(office_area);

                    int dist = std::abs( office_pos.first  - area_pos.first )
                             + std::abs( office_pos.second - area_pos.second );

                    int cost = dist;
                    if (cost < best.cost || (cost == best.cost && office_area < best.office_area)){
                        best.cost = cost;
                        best.office_id = office_id;
                        best.office_area = office_area;
                    }
                }
                else {
                    best.office_id   = office_id;
                    best.cost        = 0;
                    best.office_area = office_area;
                    break;
                }
            }
            
            total += best.cost * area_pop;
            
            Visitor::visit_office(best.office_id);
        }
    }

    return total;
}

void assign_offices() {

    best_cost = INF_COST;

    FOR_LOOP(pos0,0,NUM_AREAS-4) {
        offices[0] = pos0;
        FOR_LOOP(pos1,pos0+1,NUM_AREAS-3) {
            offices[1] = pos1;
            FOR_LOOP(pos2,pos1+1,NUM_AREAS-2) {
                offices[2] = pos2;
                FOR_LOOP(pos3,pos2+1,NUM_AREAS-1) {
                    offices[3] = pos3;
                    FOR_LOOP(pos4,pos3+1,NUM_AREAS) {
                        offices[4] = pos4;
                        
                        int cost = calculate_cost();
                        if (cost < best_cost) {
                            best_cost = cost;
                            best_offices = offices;
                        }
                    }
                }
            }
        }
    }
}

int lowest_available_area_number() {
    for (int area_id = 0; area_id < NUM_AREAS; ++area_id) {
        if (std::find(best_offices.begin(),best_offices.end(),area_id) == best_offices.end() ) {
            return area_id;
        }
    }
    // this will never, ever, happend.
    // unless something is broken, that is
    return 33;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    reader.get_line(line);
    const int T = helper::string_to_int(line);

    for (int test_num = 0; test_num < T; ++test_num) {

        if (test_num) {
            // reset state...
            population.fill(0);
            offices.fill(0);
            UnusedDetector::visited.fill(false);
        }

        reader.get_line(line);
        const int num_alive_areas = helper::string_to_int(line);

        for (int i = 0; i < num_alive_areas; ++i) {
            reader.get_line(line);
            const auto area = helper::num_array<int,3>(line);
            population[ pos_to_area_number(area[1],area[0]) ] = area[2];
        }
        
        assign_offices();

        // see which offices are not really used because
        // no populated area has it as favourite
        calculate_cost<UnusedDetector,best_offices>();
        
        // for unused offices, assign lowest index possible
        for (int i = 0; i < NUM_OFFICES; ++i) {
            if ( ! UnusedDetector::visited[i] ) {
                best_offices[i] = NUM_AREAS;
            }
        }

        for (int i = 0; i < NUM_OFFICES; ++i) {
            if ( ! UnusedDetector::visited[i] ) {
                best_offices[i] = lowest_available_area_number();
            }
        }

        std::sort(best_offices.begin(),best_offices.end());

        output.append(best_offices[0]);
        for (int i = 1; i < NUM_OFFICES; ++i) {
            output.append(' ').append(best_offices[i]);
        }
        output.endl();
    }
}

