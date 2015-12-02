
// ------------- helper.cpp begin ---------------------
// accepted flags:
//
#define NDEBUG
#include <iostream>
#include <cinttypes>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

namespace helper {

	const std::string whitespace = " \t";

	template<typename IntType = int>
	std::vector<IntType> num_vect(const std::string& s, size_t expected = 0)
	{
		std::vector<IntType> r;

		if (expected != 0)
			r.reserve(expected);
		
		for (char *data = const_cast<char*>(s.c_str())
			;*data!='\0';) {
			r.push_back(std::strtol(data,&data,10));
			while(whitespace.find(*data) != std::string::npos)
				data ++;
		}
		if (expected != 0 && r.size() != expected) {
			
		}
		return r;
	}
}

// ------------- helper.cpp end ------------------------

