#include "../../helper/helper.1.inc"

helper::LineReader<> reader;
std::string line;

constexpr int MAX_SIZE = 101;
std::array<bool,MAX_SIZE> initial_vect;
std::array<bool,MAX_SIZE> vect;

// TODO: N global
bool try_solve(const int m,const int N) {

	vect = initial_vect;
	
	for (int pos = 1, visited = 1; visited < N - 1; ++visited) {

		for (int i = 0 ; i < m ; ++i) {
			do
			{
				++pos;
				if (pos > N) 
					pos = 2;
			}
			while( vect[pos] );
		}

		if (pos == 13)
			return false;

		vect[pos] = true;
	}
	return true;
}

int inline solve(const int N) {
	for (int i = 2; ; ++i) {
		if (try_solve(i,N))
			return i;
	}
	return -1;
}

int main() {

	initial_vect[0] = true;
	initial_vect[0] = true;

	// TODO: replace std::stol with custom (1-2 digit)
	for (int N = 0 ; reader.get_line(line) && (N=std::stol(line,nullptr,10))!=0 ;) {
		
		int result;
		if (N != 13) {
			result = solve(N);
		}
		else
			result = 1;

		std::cout << result << "\n";
	}
}

