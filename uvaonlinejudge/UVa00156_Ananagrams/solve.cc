#include "../../helper/helper.1.inc"

#include <unordered_map>
#include <set>
#include <unordered_set>

using KeyType = std::string;

std::unordered_map<KeyType,std::string> seen;
std::set<std::string> ananagrams;

KeyType derive(const std::string& s) {
	KeyType tmp(s);
	std::transform(tmp.cbegin(),tmp.cend(),tmp.begin(),::tolower);
	std::sort(tmp.begin(),tmp.end());
	return tmp;
}

helper::LineReader<> reader;
std::string line;

int main() {
	
	while (reader.get_line(line)) {
		if (line[0]=='#')
			break;

		helper::for_each_word(line,[](const std::string& s) {
				std::string key( derive(s) );
				auto elem = seen.find( key );
				if (elem == seen.end()) {
					seen[key] = s;
					ananagrams.insert(s);
				}
				else {
					if (elem->second.size() > 0) {
						ananagrams.erase(elem->second);
						elem->second.clear();
					}
				}
			});
	}

	// output
	
	for ( const auto & word : ananagrams) {
		std::cout << word << "\n";
	}
}

