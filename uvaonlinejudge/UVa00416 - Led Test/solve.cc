#include "../../helper/helper.1.h"

constexpr char SA = 1 << 0,
               SB = 1 << 1,
               SC = 1 << 2,
               SD = 1 << 3,
               SE = 1 << 4,
               SF = 1 << 5,
               SG = 1 << 6;

constexpr int NUM_LEDS = 10;

constexpr std::array<char,NUM_LEDS> LED {
    SA|SB|SC|SD   |SF|SG, // 9
    SA|SB|SC|SD|SE|SF|SG, // 8
    SA|SB|SC, // 7
    SA   |SC|SD|SE|SF|SG, // 6
    SA   |SC|SD   |SF|SG, // 5
       SB|SC      |SF|SG, // 4
    SA|SB|SC|SD      |SG, // 3
    SA|SB   |SD|SE   |SG, // 2
       SB|SC, // 1
    SA|SB|SC|SD|SE|SF, // 0
};

std::array<char,NUM_LEDS> sequence;

bool matches(char led, char sample,char &dead_mask) {
    if ( (led|sample) == led && !(sample & dead_mask) )  {
        char missing = (led^sample);
        dead_mask |= missing;
        return true;
    }
    return false;
}

bool solve(int sequence_length) {

    char first_seq = sequence[0];
    
    for (int led_index = 0, limit = NUM_LEDS-sequence_length; led_index <= limit; ++led_index) {
        char dead_mask = 0;
        if (matches(LED[led_index],first_seq,dead_mask)) {
            int index;
            for (index = 1; index < sequence_length; ++index) {
                int digit = led_index + index;
                if (digit >= NUM_LEDS) break;
                if (! matches( LED[digit], sequence[index],dead_mask) )
                    break;
            }
            if (index == sequence_length) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    for (int test_num = 0; reader.get_line(line) ; ++test_num) {

        const int N = helper::string_to_int(line);
        if (N == 0) {
            break;
        }
        
        for (int i=0; i<N; ++i) {
            reader.get_line(line);
            char seq = 0;
            for (int bit = 0; bit < 7; ++bit) {
                if (line[bit] == 'Y')
                    seq |= 1 << bit;
            }
            sequence[i] = seq;
        }

        if (!solve(N))
            output.append("MIS",3);
        output.append("MATCH\n",6);
    }
}

