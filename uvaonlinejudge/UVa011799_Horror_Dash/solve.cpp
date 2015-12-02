#include <string>
#include <map>

#include "../helper/src/helper.hpp"

int main()
{
	// for each test case
	std::string line;

	std::getline(std::cin,line);
	int T = std::stol(line,nullptr,10);
	
	for (int case_num=1;case_num<=T;++case_num) {
		std::getline(std::cin,line);
		const auto speeds = helper::num_vect<int>(line);
		const int  n = speeds[0];
		int fastest = speeds[1]; 
		for (int i=2;i<=n;++i) {
			if (speeds[i] > fastest) 
				fastest = speeds[i];
		}
		std::cout << "Case "<< case_num<<": " << fastest << "\n";
	}
}

