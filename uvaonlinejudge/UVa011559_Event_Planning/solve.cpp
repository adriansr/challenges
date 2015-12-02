#include <string>
#include <map>

#include "../helper/src/helper.hpp"

int main()
{
	// for each test case
	std::string line;
	while (std::getline(std::cin,line).good()) {
		const auto definition = helper::num_vect<int>(line,4);

		int npeople = definition[0],
			budget  = definition[1],
			nhotels = definition[2],
			nweeks  = definition[3];
		
		const int INF = budget + 1;
		int min_cost = INF;

		// for each hotel
		for (int i = 0; i < nhotels; ++i) {
			std::getline(std::cin,line);
			const int price_per_room = std::stol(line,nullptr,10);

			std::getline(std::cin,line);
			const auto rooms = helper::num_vect<int>(line,nweeks);

			for (auto count : rooms) {
				if (count>=npeople) {
					min_cost = std::min( min_cost , npeople * price_per_room);
				}
			}
		}
		
		if (min_cost < INF) {
			std::cout << min_cost << "\n";
		}
		else {
			std::cout << "stay home\n";
		}
	}

}

