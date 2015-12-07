#include <string>
#include <map>
#include <array>

#include "../../helper/helper.1.inc"

/* By taking the two least significant bits of 'P', 'R', 'S' you have:
 * P = (00) 0
 * R = (10) 2
 * S = (11) 3
 */
const char kill_table[4] = { 'S', 'X', 'P', 'R' };

char inline killer_of(char victim) {
	return kill_table[ victim & 3 ];
}

bool update_checked(const std::string& src, std::string& dest,
					int x, int y, int num_cols, int num_rows) {
	
	size_t pos = x + y * num_cols;
	const char *src_ptr = &src[pos];
	char current = *src_ptr,
		 killer  = killer_of(current);

	if (             (x>0 && killer==src_ptr[-1])
	  || ( (x+1<num_cols) && killer==src_ptr[+1] )
	  ||          ( (y>0) && killer==src_ptr[-num_cols] )
	  || ( (y+1<num_rows) && killer==src_ptr[+num_cols] )) {
		
		dest[pos] = killer;
		return true;
	}
	//already
	//dest[pos] = current;
	return false;
}

bool update_unchecked(const std::string& src, std::string& dest,
					  int x, int y, int num_cols) {
	
	size_t pos = x + y * num_cols;
	const char *src_ptr = &src[pos];
	char current = *src_ptr,
		 killer  = killer_of(current);

	if (             (       killer==src_ptr[-1])
	  || (                   killer==src_ptr[+1] )
	  ||          (          killer==src_ptr[-num_cols] )
	  || (                   killer==src_ptr[+num_cols] )) {
		
		dest[pos] = killer;
		return true;
	}
	//already
	//dest[pos] = current;
	return false;
}


bool simulate(std::string& grid, int num_rows, int num_cols) {
	std::string source(grid);
	
	bool continue_flag = false;

	for (int x = 0;x<num_cols;++x) {
		continue_flag = update_checked(source,grid,x,0,num_cols,num_rows) || continue_flag;
		continue_flag = update_checked(source,grid,x,num_rows-1,num_cols,num_rows) || continue_flag;
	}
	for (int y = 1;y<num_rows-1;++y) {
		continue_flag = update_checked(source,grid,0,y,num_cols,num_rows) || continue_flag;
		continue_flag = update_checked(source,grid,num_cols-1,y,num_cols,num_rows) || continue_flag;
	}

	for (int y=1;y<num_rows-1;++y) {
		for (int x=1;x<num_cols-1;++x) {
			continue_flag = update_unchecked(source,grid,x,y,num_cols) || continue_flag;
		}
	}
	return continue_flag;
}

int main()
{
	// for each test case
	std::string line;
	
	std::getline(std::cin,line);
	int num_tests = std::stol(line,nullptr,10);

	for(int test_case=1;test_case<=num_tests;++test_case) {

		std::getline(std::cin,line);
		const auto settings = helper::num_array<int,3>(line);
		
		const int num_rows = settings[0],
			  	  num_cols = settings[1],
				  num_days = settings[2];
		
		// using a string because it's holding chars anyway
		// also it's faster to use a single dimension
		std::string grid;
		grid.reserve( num_rows * num_cols);
		for (int row = 0 ; row < num_rows ; ++row) {
			std::getline(std::cin,line);
			assert(line.size() == num_cols);
			grid.append(line);
		}
		
		for (int day = 0; day < num_days ; ++day) {
			if (!simulate(grid,num_rows,num_cols)) {
				// early-exit (optimization)
				break;
			}
		}
		
		if (test_case>1)
			std::cout << "\n";

		for (int row = 0 ; row < num_rows ; ++row) {
			std::cout << grid.substr(num_cols*row,num_cols) << "\n";
		}
	}
}
