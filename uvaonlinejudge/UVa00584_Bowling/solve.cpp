#include <string>
#include <map>
#include <iostream>

//#include "../helper/src/helper.hpp"

int main()
{
	// for each test case
	std::string line;

	for (int test_num=1;std::getline(std::cin,line).good();++test_num) {

		if (line == "Game Over")
			break;

		int total = 0;
		
		int multiplier[2] {1,1};
		int counter = 1;
		for (size_t pos = 0;pos<line.size();pos+=2,counter++) {
			char first_roll = line[pos];
			
			if (counter>10) {
				multiplier[0] --;
				multiplier[1] --;
			}
			if ( first_roll == 'X') {
				total += 10 * multiplier[0];
				multiplier[0] = multiplier[1] + 1;
				multiplier[1] = 2;
			}
			else {
				pos += 2;
				char second_roll = pos<line.size()? line[pos] : '0'; 
				
				int first_score = first_roll - '0',
					second_score;
				bool is_spare = second_roll == '/';
				if (is_spare) {
					second_score = 10 - first_score;
				}
				else {
					second_score = second_roll - '0';
				}

				total += first_score*multiplier[0] + second_score*multiplier[1];
				multiplier[0] = is_spare? 2 : 1;
				multiplier[1] = 1;
			}
		}

		std::cout << total << "\n";
	}
}

