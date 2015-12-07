#include "../../helper/helper.1.inc"
#include <unordered_map>

helper::LineReader<> reader;
std::string line;

size_t extract_name(const std::string& line, std::string& name) {
	size_t pos;
	for (pos = 1;pos < 10 && line[pos] != ' '; ++pos)
		;
	name.assign(line,0,pos);
	for (; line[pos] == ' '; ++pos)
		;

	return pos;
}

int main() {
	
	std::string output;
	output.reserve(4096);

	reader.get_line(line);

	const auto desc = helper::num_array<int,2>(line);
	const int num_arrays = desc[0],
		  	  num_refs   = desc[1];
	
	std::unordered_map<std::string,std::vector<int>> array_desc;
	
	std::string name;
	name.reserve(10);

	for (int i = 0 ; i < num_arrays ; ++i) {
		reader.get_line(line);
		size_t pos = extract_name(line,name);
		const auto desc = helper::num_vector<int>(line,pos);
		const int B  = desc[0],
			  	  Cd = desc[1],
				  D  = desc[2];

		std::vector<int> C;
		C.resize(D + 1);
		
		C[D] = Cd;
		int C0 	  = B - Cd * desc[1 + 2*D];
		for (int i = D - 1; i >= 1 ; --i ) {
			const int Li_1 = desc[3 + 2*i],
				  	  Ui_1 = desc[4 + 2*i];
			C[i] = C[i+1] * ( Ui_1 - Li_1 + 1);
			C0 -= C[i] * desc[1 + 2*i];
		}
		C[0] = C0;
		array_desc[ name ] = C;
	}

	for (int i = 0; i < num_refs; ++i) {
		reader.get_line(line);
		size_t pos = extract_name(line,name);
		const auto idx = helper::num_vector<int>(line,pos);
		const auto& C = array_desc[name];

		size_t result = C[0];

		output.append(name);
		output.push_back('[');
		for (int i=0,lim=idx.size()-1;i<=lim;++i) {
			auto val = idx[i];
			output.append(std::to_string(val));
			if (i != lim) {
				output.append(", ",2);
			}
			result += C[i+1] * val;
		}
		output.append("] = ",4);
		output.append(std::to_string(result));
		output.push_back('\n');
	}

	write(1,output.data(),output.size());
}

