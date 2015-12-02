#include <iostream>
#include <string>
#include <map>

int main()
{
	std::string line;
	std::getline(std::cin,line);

	unsigned long n = std::strtoul(line.c_str(),nullptr,10);
	
	std::map<std::string,unsigned long> conquests;

	for (unsigned long i = 0 ; i < n && std::getline(std::cin,line).good() ; ++i ) {
		conquests[ line.substr(0,line.find(' ')) ] ++;
	}

	for (const auto& it : conquests) {
		printf("%s %lu\n", it.first.c_str(), it.second);
	}

}

