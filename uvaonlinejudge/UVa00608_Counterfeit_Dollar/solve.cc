//#include "../../helper/helper.1.inc"

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <unistd.h>

//#define NDEBUG
#include <cassert>

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

template<size_t BUFFER_SIZE = 4096>
class LineReader {
	public:
		LineReader() {
			buffer_.resize(BUFFER_SIZE);
			eof_ = !fill_buffer();
		}

		bool get_line(std::string& s) {
			if (eof_) {
				return false;
			}

			size_t pos;
			//std::cerr << "used: " << used_ << "/" << buffer_.size() << "\n";
			while ( (pos=buffer_.find('\n',used_)) == std::string::npos) {
				if (eof_) {
					pos = buffer_.size();
					if (used_ < pos) {
						break;
					}
					return false;
				}
				if (!fill_buffer()) {
					eof_ = true;
				}
			}

			s.assign(buffer_,used_,pos-used_);
			//std::cerr << "s is " << s << "\n";
			used_ = pos+1;
			return true;
		}

	private:
		bool fill_buffer() {
			if (lastRead_) return false;

			size_t len = buffer_.size();
			size_t fit,remain;
			if (used_>0) {
				remain = len - used_;
				if (used_ < (len>>1)) {
					memcpy(&buffer_[0],&buffer_[used_],remain);
				}
				else {
					memmove(&buffer_[0],&buffer_[used_],remain);
				}
				fit = used_;
				used_ = 0;
			}
			else {
				fit = len;
				remain = 0;
			}
			auto ret = ::read(0,&buffer_[remain],fit);
			//std::cerr << "Reading " << fit << "/" << len << " : " << ret << "\n";
			if (ret == fit) {
				return true;
			}
			lastRead_ = true;
			if (ret>0) {
				buffer_.resize(remain + ret);
				return true;
			}
			buffer_.resize(remain);
			return false;
		}

		bool   eof_,lastRead_=false;
		size_t used_ = 0;
		std::string buffer_;
};


} // namespace helper

#define NCOINS 12

#define	FLAG_IS_LIGHT 0
#define	FLAG_IS_HEAVY 1
#define FLAG_IS_EVEN  2
#define NUM_FLAGS	  3

struct C {
	int flags[NCOINS][NUM_FLAGS];
} clear_coins, coins;

void set(const char *s, int flag1, int flag2) {
	char c;
	while ( (c=*s++) != ' ') {
		coins.flags[ c - 'A' ][flag1] ++;
	}
	while ( (c=*s++) != ' ') {
		coins.flags[ c - 'A' ][flag2] ++;
	}
}

helper::LineReader<> reader;
std::string line;

#define MSG_LEN 43
//                          1234567890123456789012345678901234567890123
char msg[3][MSG_LEN+1] = { "K is the counterfeit coin and it is light.\n" ,
   			   	           "K is the counterfeit coin and it is heavy.\n"}; 

void output(int coin_idx, int flag) {
	char *sel_msg = msg[flag]; // heavy by default ?
	sel_msg[0] = 'A' + coin_idx;
	::write(1,sel_msg,MSG_LEN);
}

void process_weight(int test_num) {
	reader.get_line(line);
	//std::cout << "Test case " << test_num << " : " << line << "\n";
	char opcode = line[ line.size()-2 ];
	int flag1,flag2;

	switch(opcode) {
		case 'e':
			flag1 = flag2 = FLAG_IS_EVEN;
			break;

		case 'w':
			flag1 = FLAG_IS_LIGHT;
			flag2 = FLAG_IS_HEAVY;
			break;

		case 'u':
			flag1 = FLAG_IS_HEAVY;
			flag2 = FLAG_IS_LIGHT;
			break;
	}
	set(line.data(),flag1,flag2);
}

int main() {
	
	reader.get_line(line);
	
	const int num_cases = std::stol(line,nullptr,10);
	// for each test case
	for (int test_num=0;test_num < num_cases;++test_num) {
		if (test_num) {
			//reader.get_line(line);
			coins = clear_coins;
		}
		process_weight(test_num);
		process_weight(test_num);
		process_weight(test_num);
		
#if 0
		for (int i=0;i<NCOINS;++i) {
			std::cout << char('A' + i);
		}
		std::cout << "\n"; 
		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_EVEN]);
		}
		std::cout << "\n"; 

		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_LIGHT]);
		}
		std::cout << "\n"; 

		for (int i=0;i<NCOINS;++i) {
			std::cout << char('0' + coins.flags[i][FLAG_IS_HEAVY]);
		}
		std::cout << "\n"; 
#endif


		struct {
			int idx, max;
		} light, heavy;

		light.max = 0;
		heavy.max = 0;

		for (int i = 0; i < NCOINS; ++i) {
			const auto & r = coins.flags[i];
			if ( !r[FLAG_IS_EVEN] ) {
				int level;
				if ((level=r[FLAG_IS_HEAVY]) > heavy.max) {
					heavy.idx = i;
					heavy.max = level;
				}
				if ((level=r[FLAG_IS_LIGHT]) > light.max) {
					light.idx = i;
					light.max = level;
				}
			}
		}
		
		if (light.max > heavy.max) {
			output(light.idx, FLAG_IS_LIGHT);
		}
		else if (light.max < heavy.max) {
			output(heavy.idx, FLAG_IS_HEAVY);
		}
		else {
		// fallback if coin not referenced
			for (int i = 0; i < NCOINS; ++i) {
				const auto & r = coins.flags[i];
				if ( !r[FLAG_IS_EVEN] ) {
					output(i,FLAG_IS_HEAVY); 
					break;
				}
			}
		}
	}
}

