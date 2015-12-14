#include "../../helper/helper.1.inc"

helper::LineReader<> reader;
std::string line;

bool shittyOrdering(char a, char b) {
	char a_low = 0x20|a,// ::tolower(a),
		 b_low = 0x20|b;//::tolower(b);

	return a_low < b_low || (a_low == b_low && a < b);
}

int main() {
	
	reader.get_line(line);
	std::string output;
	output.reserve(4096);

	const int N = std::stol(line,nullptr,10);

	for (int test_case = 1 ; test_case <= N && reader.get_line(line); ++test_case) {

		//line.push_back('\n');
		//size_t len = line.size();
		//const char * data = line.data();
		auto begin = line.begin(),
			 end   = line.end();
		std::sort(begin,end,shittyOrdering);
		
		do
		{
			//::write(1,data,len);
			output.append(line);
			output.push_back('\n');
		}
		while (std::next_permutation(begin,end,shittyOrdering));
	}

	write(1,output.data(),output.size());
}

