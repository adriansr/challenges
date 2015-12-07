#include <string>
#include <map>
#include <array>

//#include "../../helper/helper.1.inc"
#include <cstring>

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

#include <unistd.h>

//#define NDEBUG
//#include <cassert>

namespace helper {

namespace {
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
	
	//assert( inserter.ok() );
}

} // anonymous-namespace 

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

std::array<FieldType,FixedLength> num_array(const std::string& s) {
	
	std::array<FieldType,FixedLength> retval{};
	using Container = decltype(retval);
	split_line<FieldType,Container,FixedSizeInserter<Container>,Convert,IsDelimiter>(s,&retval);
	//report("num_array",retval);
	return retval;
}

template<typename FieldType,
		 FieldType (*Convert)(const std::string&) = & ConvertStr<FieldType>,
		 bool (*IsDelimiter)(char) = IsSpace >

std::vector<FieldType> num_vector(const std::string& s) {
	
	std::vector<FieldType> retval;
	using Container = decltype(retval);
	split_line<FieldType,Container,VariableSizeInserter<Container>,Convert,IsDelimiter>(s,&retval);
	//report("num_vector",retval);
	return retval;
}


} // namespace helper
/* By taking the two least significant bits of 'P', 'R', 'S' you have:
 * P = (00) 0
 * R = (10) 2
 * S = (11) 3
 */
const char kill_table[4] = { 'S', 'X', 'P', 'R' };

char inline killer_of(char victim) {
	return kill_table[ victim & 3 ];
}

char g_source[ 100 * 100 ];
char g_saved [ 100 * 100 ];
int num_cols;
int num_rows;

template< bool CHECK_LEFT = true,
		  bool CHECK_UP = true,
		  bool CHECK_RIGHT = true,
		  bool CHECK_DOWN = true>
bool update_partial(int x, int y) {
	
	size_t pos = x + y * num_cols;
	const char *src_ptr = &g_saved[pos];
	char current = *src_ptr,
		 killer  = killer_of(current);

	if ( (CHECK_LEFT  && killer==src_ptr[-1])
	  || (CHECK_RIGHT && killer==src_ptr[+1] )
	  || ( CHECK_UP   && killer==src_ptr[-num_cols] )
	  || ( CHECK_DOWN && killer==src_ptr[+num_cols] )) {
		
		g_source[pos] = killer;
		return true;
	}
	//already
	//dest[pos] = current;
	return false;
}

bool inline update_unchecked(size_t pos) {
	
	const char *src_ptr = &g_saved[pos];
	char current = *src_ptr,
		 killer  = killer_of(current);

	if (             (       killer==src_ptr[-1])
	  || (                   killer==src_ptr[+1] )
	  ||          (          killer==src_ptr[-num_cols] )
	  || (                   killer==src_ptr[+num_cols] )) {
		
		g_source[pos] = killer;
		return true;
	}
	//already
	//dest[pos] = current;
	return false;
}


bool inline update_checked(size_t pos, int x, int y) {
	
	const char *src_ptr = &g_saved[pos];
	char current = *src_ptr,
		 killer  = killer_of(current);

	if (             (x>0 && killer==src_ptr[-1])
	  || (x+1 < num_cols && killer==src_ptr[+1] )
	  ||          ( y>0 && killer==src_ptr[-num_cols] )
	  || ( y+1< num_rows &&  killer==src_ptr[+num_cols] )) {
		
		g_source[pos] = killer;
		return true;
	}
	//already
	//dest[pos] = current;
	return false;
}


bool simulate() {
	memcpy((void*)g_saved,g_source,num_rows * num_cols);

	bool continue_flag = false;

#define CCHECK(X...) continue_flag |= X ;
	
	// L U R D
	CCHECK(update_partial<0,0,1,1>(0,0));
	CCHECK(update_partial<1,0,0,1>(num_cols-1,0));
	CCHECK(update_partial<0,1,1,0>(0,num_rows-1));
	CCHECK(update_partial<1,1,0,0>(num_cols-1,num_rows-1));

	for (int x = 1;x<num_cols-1;++x) {
		CCHECK(update_partial<1,0,1,1>(x,0));
		CCHECK(update_partial<1,1,1,0>(x,num_rows-1));
	}
	for (int y = 1;y<num_rows-1;++y) {
		CCHECK(update_partial<0,1,1,1>(0,y));
		CCHECK(update_partial<1,1,0,1>(num_cols-1,y));
	}

	int pos = num_cols + 1;
	for (int y=1;y<num_rows-1;++y,pos+=2) {
		for (int x=1;x<num_cols-1;++x,++pos) {
			CCHECK(update_unchecked(pos));
		}
	}
	return continue_flag;
}


bool simulate_slow() {
	memcpy((void*)g_saved,g_source,num_rows * num_cols);

	bool continue_flag = false;

#define CCHECK(X...) continue_flag |= X ;
	
	// L U R D
	int pos = 0;
	for (int y=0;y<num_rows;++y) {
		for (int x=0;x<num_cols;++x) {
			CCHECK(update_checked(pos++,x,y));
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
		
		num_rows = settings[0];
		num_cols = settings[1];
		const int num_days = settings[2];
		
		size_t pos = 0;
		for (int row = 0 ; row < num_rows ; ++row) {
			std::getline(std::cin,line);
			//assert(line.size() == num_cols);
			memcpy(&g_source[pos],line.data(),num_cols);
			pos += num_cols;
		}
		
		if (num_rows>1 && num_cols>1) {
			for (int day = 0; day < num_days ; ++day) {
				if (!simulate()) {
					// early-exit (optimization)
					break;
				}
			}
		}
		else if (num_rows>1 || num_cols>1) {
			for (int day = 0; day < num_days ; ++day) {
				if (!simulate_slow()) {
					// early-exit (optimization)
					break;
				}
			}
		}

		if (test_case>1)
			write(1,"\n",1);
			//std::cout << "\n";
		
		std::string out;
		out.resize(num_cols+1);
		for (int row = 0 ; row < num_rows ; ++row) {
			memcpy(&out[0],&g_source[row * num_cols],num_cols);
			out[num_cols] = '\n';
			//std::cout << out << "\n";
			write(1,out.data(),out.size());
		}
	}
}
