#include <string>
#include <map>
#include <array>

#include "../helper/src/helper.hpp"

const int NUM_CELLS = 100;

int main()
{
	// for each test case
	std::string line;
	
	std::getline(std::cin,line);
	int num_tests = std::stol(line,nullptr,10);

	for(int test_case=1;test_case<=num_tests;++test_case) {

		std::getline(std::cin,line);
		const auto settings = helper::num_vect<int>(line,3);
		
		const int num_players = settings[0],
			  	  num_traps   = settings[1],
				  num_rolls	  = settings[2];
		
		std::vector<int> board;
		std::vector<int> players;

		board.resize(NUM_CELLS);
		players.resize(num_players);
		
		for (int i = 0 ; i < num_traps ; ++i) {
			std::getline(std::cin,line);
			const auto trap = helper::num_vect<int>(line,2);
			if (trap[0] < NUM_CELLS && board[trap[0]-1]==0 && trap[1]>0 && trap[1]<=NUM_CELLS)
				board[trap[0]-1] = trap[1];
		}
		
		bool game_running = true;

		for (int i = 0, p = 0; i < num_rolls; ++i, p = (p + 1) % num_players) {

			std::getline(std::cin,line);

			if (game_running) {
				int roll = /*std::stol(line,nullptr,10);*/line[0] - '0';
				int pos = players[p] += roll;

				if (pos < NUM_CELLS - 1 && board[pos]>0) {
					pos = players[p] = board[pos] - 1;
				}
				if (pos >= NUM_CELLS-1)	{
					game_running = false;
					players[p] = NUM_CELLS-1;
				}
			}
		}
		
		//if (test_case>1)
		//		std::cout << "\n";

		for (int p = 0; p < num_players; ++p) {
			std::cout << "Position of player " << (p+1) << " is " << (players[p]+1) << ".\n";
		}
	}
}
