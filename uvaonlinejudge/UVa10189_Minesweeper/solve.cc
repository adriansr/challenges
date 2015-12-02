#include <string>
#include <map>

#include "../helper/src/helper.hpp"

constexpr int MINE = -10;
using VectorType = std::vector< std::vector<int> >;

void update(VectorType& v, int x, int y)
{
	const bool has_Xmin1 = x>0;
	const bool has_Xplus1 = x+1 < v[y].size();

	v[y][x] = MINE;
	if (y>0) {
		if (has_Xmin1)
			v[y-1][x-1] ++;
		v[y-1][x] ++;
		if (has_Xplus1)
			v[y-1][x+1] ++;
	}

	if (has_Xmin1)
		v[y][x-1] ++;
	if (has_Xplus1)
		v[y][x+1] ++;

	if (y+1 < v.size()) {
		if (has_Xmin1)
			v[y+1][x-1] ++;
		v[y+1][x] ++;
		if (has_Xplus1)
			v[y+1][x+1] ++;
	}
}

int main()
{
	// for each test case
	std::string line;
	
	for(int count=1;std::getline(std::cin,line).good();count++) {
		const auto dimensions = helper::num_vect(line,2);
		
		if (dimensions[0]==0 || dimensions[1]==0)
			break;
		
		VectorType field;

		field.resize(dimensions[0]);
		
		for (int y=0;y<dimensions[0];++y) {
			field[y].resize(dimensions[1]);
		}

		for (int y=0;y<dimensions[0];++y) {
			std::getline(std::cin,line);
			assert( line.size() == dimensions[1] );
			for (int x=0;x<dimensions[1];++x) {
				if (line[x]=='*') {
					update(field,x,y);
				}
			}
		}
		
		if (count>1)
			std::cout << "\n";

		std::cout << "Field #" << count << ":\n";

		for (int y=0;y<dimensions[0];++y) {
			for (int x=0;x<dimensions[1];++x) {
				if (field[y][x]<0)
					std::cout << '*';
				else
					std::cout << field[y][x];
			}
			std::cout << "\n";
		}
	}
}

