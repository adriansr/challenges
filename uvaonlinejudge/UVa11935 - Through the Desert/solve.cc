#include "../../helper/helper.1.h"
#include <cstring>

class JeepSimulator {
    public:

        bool init(const std::string& line) {
            auto p = parse_consumption(line);
            consumption_ = p.second;
            return p.first == 0 && p.second!=0;
        }

        bool onEvent(const std::string& line) {
            auto p = parse_line(line);
            update_fuel(p.first);
            
            switch( line[p.second] ) {
                case 'F': // Fuel leak
                    consumption_ = parse_consumption(line).second;
                    break;
                
                case 'L':
                    num_leaks_ ++;
                    break;

                case 'M':
                    num_leaks_ = 0;
                    break;

                case 'G':
                    if ( line[p.second+1] == 'a' ) {
                        fuel_used_ = 0;
                    }
                    else {
                        return false;
                    }
            }

            return true;
        }

        float fuel_required() const {
            return max_fuel_used_;
        }

    private:
        
        void update_fuel(int km) {
            int elapsed = km - position_;
            fuel_used_ += (elapsed * consumption_ / 100.0) + num_leaks_*elapsed;
            position_ = km;
            if (fuel_used_ > max_fuel_used_) {
                max_fuel_used_ = fuel_used_;
            }
        }

        std::pair<int,int> parse_consumption(const std::string& line) {
            auto p = parse_line(line);
            size_t conspos = line.rfind(' ');
            int cons = std::strtol(line.c_str()+conspos+1,nullptr,10);
            return std::make_pair(p.first,cons);
        }

        std::pair<int,size_t> parse_line(const std::string& line) {
            size_t pos = 0;
            int num = std::stoi(line,&pos,10);
            return std::make_pair(num,pos+1);
        }

        int consumption_ = 0,
               position_ = 0,
              num_leaks_ = 0;

        float     fuel_used_ = 0,
              max_fuel_used_ = 0;
};

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    char buf[128];
    while (reader.get_line(line)) {
        
        JeepSimulator sim;
        if (!sim.init(line)) {
            break;
        }

        do {
            reader.get_line(line);
        }
        while( sim.onEvent(line) );
        
        std::snprintf(buf,128,"%.03f\n", sim.fuel_required());
        output.append(buf);
    }
}

