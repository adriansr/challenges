/* solve2.cc / Car Park Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 */
#include "helper.1.h"
#include <map>

/*enum EventType {
    Entrance,
    Departure
};*/

typedef int DayTime; // time in seconds since 00:00 in current day
typedef int Day; // Days are identified as an integer "24.09.2015" -> 20150924

struct Timestamp {
    Day     day;
    DayTime time;
};
constexpr DayTime END_OF_DAY = 60*60*24;

// struct holding stats for a single day
struct DayStats {
    double earned; // total money earned this day
    int peak_cars;
    std::map<DayTime,int> events;
};

size_t read_number(const std::string& s, size_t offset, int *pResult) {
    int result;
    size_t pos, limit;
    char c;
    for (result = 0, pos = offset, limit = s.size()
        ; pos < limit && (c=s[pos])>='0' && c <= '9'
        ; ++pos) {

        result = result * 10 + c - '0';
    }

    *pResult = result;
    return pos;
}

// reads a date from the string at offset
void read_date(const std::string& s, size_t offset, Timestamp* pTimestamp) {
    
    const auto date = helper::num_array<int,6,helper::ConvertStr<int>,&helper::IsNotDigit>
                      (s,offset);
    
    pTimestamp->day  = date[2]*10000 + date[1]*100 + date[0];
    pTimestamp->time = (date[3]*60 + date[4])*60 + date[5];
}

void read_record(const std::string& s, Timestamp *pEntry, Timestamp *pLeave) {

    size_t pos = s.find('-');
    if (pos != s.npos) {
        read_date(s,0,pEntry);
        read_date(s,pos+1,pLeave);
    }
    else
        throw "Broken record!";
}

std::string day_to_string(Day day) {
    std::string buf,tmp;
    buf.resize(12);
    auto size = sprintf(&buf[0],"%02d.%02d.%04d",
                        (day % 100),
                        (day / 100) % 100,
                        (day / 10000));

    buf.resize(size);
    return buf;
}

double cost(int totalTime) {
    constexpr int HOUR  = 60*60,
                  QUART = 15*60;

    if ( totalTime <= HOUR) {
        return 2;
    }
    else {
        int quarts = totalTime / QUART,
            rem    = totalTime % QUART;

        return static_cast<double>(quarts) * 0.5 + (rem? 0.5 : 0.0);
    }
}

int main() {

    // fast input from stdin
    helper::LineReader<> reader;
    std::string line;
	
    // fast output to stdout
	std::string output;
	output.reserve(4096);
    
    // read number of records C
    reader.get_line(line);
    const int C = helper::string_to_int(line);
    
    std::map<Day,DayStats> days;

    for (int record_count = 0; record_count < C ; ++record_count) {

        reader.get_line(line);
        
        Timestamp entry, leave;

        read_record(line, &entry, &leave);
        
        auto& entry_day = days[entry.day];
        auto& leave_day = days[leave.day];

        ++ entry_day.events[entry.time];
        -- leave_day.events[leave.time];

        double total_cost;
        if (entry.day == leave.day) {
            total_cost = cost(leave.time - entry.time);
        }
        else {
            int full_days = leave.day - entry.day - 1;
            total_cost = cost( leave.time + END_OF_DAY*full_days + END_OF_DAY - entry.time );
        }
        
        leave_day.earned += total_cost;
    }
    
    int ncars = 0;
    int total_peak = -1;
    Day total_peak_day = 0;

    for ( auto& day : days ) {
        auto & stats = day.second;
        stats.peak_cars = ncars;
        for ( const auto& event : stats.events) {
            ncars += event.second;
            if (ncars > stats.peak_cars) {
                stats.peak_cars = ncars;
                if (ncars > total_peak) {
                    total_peak = ncars;
                    total_peak_day = day.first;
                }
            }
        }  
    }

    double total_earned = 0;
    for ( const auto& day : days ) {
        if (day.second.earned>0) {
            total_earned += day.second.earned;
            
            // I know, it's ugly, but 10x faster than iostreams
            
            output.append( day_to_string(day.first) );
            output.push_back(' ');
            output.append( std::to_string( day.second.peak_cars ) );
            output.push_back(' ');
            int cost_intpart = static_cast<int>(day.second.earned);
            output.append( std::to_string( cost_intpart ) );
            output.append(((day.second.earned - cost_intpart) > 0.0)? ".5\n" : ".0\n");
        }
    }

    output.append("PEAK ");
    output.append( std::to_string(total_peak) );
    output.append(" AT ");
    output.append( day_to_string( total_peak_day ) );
    output.append("\nTOTAL ");
    int cost_intpart = static_cast<int>(total_earned);
    output.append( std::to_string( cost_intpart ) );
    output.append(((total_earned - cost_intpart) > 0.0)? ".5\n" : ".0\n");

	write(1,output.data(),output.size());
}

