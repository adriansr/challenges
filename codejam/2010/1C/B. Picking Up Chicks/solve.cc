#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

#define NDEBUG
#include <cassert>

bool IsSpace(char c) {
	return c == ' ';
}

template<typename IntType>
IntType ConvertStr(const std::string& s)
{
	return std::stol(s);
}

template<class Container>
class FixedSizeInserter {
	public:
		FixedSizeInserter(Container *p) : container_(p), size_(p->size()), pos_(0), ok_(true) {}

		void insert(const typename Container::value_type& value) {
			if (ok_ && (ok_=pos_ < size_))
				(*container_)[pos_++] = value;
		}
		
		bool ok() const { return ok_; }

	private:
		Container *container_;
		size_t     size_,
				   pos_;
		bool	   ok_;
};

template<class Container>
class VariableSizeInserter {
	public:
		VariableSizeInserter(Container *p) : container_(p) {}

		void insert(const typename Container::value_type& value) {
			container_->push_back(value);
		}
		
		bool ok() const { return true; }

	private:
		Container *container_;
};

template<typename FieldType,
		 class Container,
		 class Inserter = FixedSizeInserter<Container>,
		 FieldType (*Convert)(const std::string&) = & ConvertStr<FieldType>,
		 bool (*IsDelimiter)(char) = IsSpace >
void split_line(const std::string &s, Container *pcontainer) {
	
	Inserter inserter(pcontainer);

	size_t pos = 0,
		   limit = s.size();

	while (inserter.ok() && pos<limit) {
		// skip whitespace
		for (;pos<limit && IsDelimiter(s[pos]);++pos)
			;
		if (pos>=limit)
			break;

		size_t base = pos;
		for (pos=pos+1;pos<limit && !IsDelimiter(s[pos]);++pos)
			;
	
		inserter.insert(Convert(s.substr(base,pos-base)));
	}
	
	assert( inserter.ok() );
}

template<class T>
void report(const char *name, const T& t) {
	std::cerr << name << "[" << t.size() << "]: ";
	for (const auto& elem : t) {
		std::cerr << elem << " ";
	}
	std::cerr << "\n";
}

template<typename FieldType,
		 size_t FixedLength,
		 FieldType (*Convert)(const std::string&) = & ConvertStr<FieldType>,
		 bool (*IsDelimiter)(char) = IsSpace >

std::array<FieldType,FixedLength> line_array(const std::string& s) {
	
	std::array<FieldType,FixedLength> retval{};
	using Container = decltype(retval);
	split_line<FieldType,Container,FixedSizeInserter<Container>,Convert,IsDelimiter>(s,&retval);
	//report("line_array",retval);
	return retval;
}

template<typename FieldType,
		 FieldType (*Convert)(const std::string&) = & ConvertStr<FieldType>,
		 bool (*IsDelimiter)(char) = IsSpace >

std::vector<FieldType> line_vector(const std::string& s) {
	
	std::vector<FieldType> retval;
	using Container = decltype(retval);
	split_line<FieldType,Container,VariableSizeInserter<Container>,Convert,IsDelimiter>(s,&retval);
	//report("line_vector",retval);
	return retval;
}

struct Chick {
	int distance_to_barn = 0,
		speed = 0;
	bool makes_it = false;
};

int main() {
	std::string line;
	std::getline(std::cin,line);

	int num_cases = std::stoi(line);

	for (int test_case = 1 ; test_case <= num_cases ; ++test_case ) {
		std::getline(std::cin,line);

		const auto params = line_vector<int>(line);
		const auto N = params[0]; // num chicken
		const auto K = params[1]; // target 
		const auto B = params[2]; // barn position
		const auto T = params[3]; // time
		
		std::vector<Chick> chicken;
		chicken.resize(N);

		std::getline(std::cin,line);
		const auto start_positions = line_vector<int>(line);
		assert( start_positions.size() == N);
		for (int i=0;i<N;++i) {
			chicken[i].distance_to_barn = B - start_positions[i];
		}

		std::getline(std::cin,line);
		const auto velocities = line_vector<int>(line);
		assert( velocities.size() == N);
		for (int i=0;i<N;++i) {
			chicken[i].speed = velocities[i];
			chicken[i].makes_it = chicken[i].distance_to_barn - T * chicken[i].speed <= 0;
		}
		
		// reverse-sort from closer to B position to furthest 
		std::sort(chicken.begin(),chicken.end(),[](const Chick& a, const Chick& b){return a.distance_to_barn < b.distance_to_barn;});

		int count_swaps = 0,
			successful_chicken = 0,
			slow_ahead = 0;

		for ( auto& chick : chicken) {
			if (chick.makes_it) {
				count_swaps += slow_ahead;
				if ( ++successful_chicken == K) {
					break;
				}
			}
			else {
				++ slow_ahead;
			}
		}

		std::cout << "Case #" << test_case << ": ";
		if (successful_chicken == K) {
			std::cout << count_swaps;
		}
		else {
			std::cout << "IMPOSSIBLE";
		}

		std::cout << "\n";

	}
}

