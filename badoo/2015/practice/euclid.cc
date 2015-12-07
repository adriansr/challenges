#include "../../../helper/helper.1.inc"

int gcd(int a, int b) {

    while ( a && b ) {
        
        int remainder = a % b;

        a = b;
        b = remainder;
    }

    return a? a : b;
}

int main() {
    std::string output;
    output.reserve(4096);

    helper::LineReader<> reader;
    std::string line;

    reader.get_line(line);
    const int NUM_TESTS = std::stol(line,nullptr,10);
    
    for (int test_count = 0 ; test_count < NUM_TESTS ; ++test_count) {
        
        reader.get_line(line);
        const auto params = helper::num_array<int,3>(line);
        
        int A,B,G = params[2];

        // want A >= B
        if (params[0] >= params[1]) {
            A = params[0];
            B = params[1];
        }
        else {
            A = params[1];
            B = params[0];
        }
        
        int actual_gcd = gcd(A,B);

        if (actual_gcd == G) {
            output.append("OK\n",3);
        }
        else {
            output.append(std::to_string(actual_gcd));
            output.push_back('\n');
        }


    }

    write(1,output.data(),output.size());
}

