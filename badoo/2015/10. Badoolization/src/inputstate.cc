#include "inputstate.h"
// my ugly set of helper functions for programming challenges
// performance oriented IO from the console
// since this challenge is not based on speed I've refrained from
// using them anymore, except for the array/vector reading rutines.
#include "helper.1.h"

#include <algorithm> // for_each
#include <cctype> // std::tolower

namespace badoolization {

    bool InputState::read_from(std::istream& stream) {
        
        std::string line;

        if (! std::getline(stream,line).good() ) {
            std::cerr << "EOF while reading number of lines\n";
            return false;
        }

        const int num_resources = std::stoi(line,nullptr,10);

        for (int i = 0; i < num_resources; ++i) {
            if (! std::getline(stream,line).good() ) {
                std::cerr << "EOF while reading resources\n";
                return false;
            }

            auto space_pos = line.find(' ');
            if (space_pos == line.npos) {
                std::cerr << "Wrong resource format\n";
                return false;
            }
            
            std::string key = line.substr(0,space_pos);
            std::for_each(key.begin(),key.end(),[](char& c) { c = std::tolower(c); } );
            
            if (resources.find(key) != resources.end()) {
                std::cerr << "Duplicate resource " << key << " in input\n";
                return false;
            }
            
            try {
                resources[key] = std::stoi(line.substr(space_pos+1),nullptr,10);
            }
            catch(const std::exception& e) {
                std::cerr << "Error parsing number in input: " << e.what() << "\n";
                return false;
            }
        }
        return true;
    }

	void InputState::log(std::ostream& stream, const char * name) const {
		stream << name << " state (" << resources.size() << " resources):\n";
		std::string padding;
		padding.resize(8,' ');
		for (const auto it : resources) {
			std::string snum( std::to_string(it.second) );
			if (snum.size()<padding.size()) {
				stream << padding.substr(0,padding.size() - snum.size());
			}
			stream << snum << " x " << it.first << "\n";
		}
		stream << "\n";
	}


} // namespace badoolization
