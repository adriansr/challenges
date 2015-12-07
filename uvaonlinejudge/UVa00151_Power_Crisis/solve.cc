// #include "../../helper/helper.1.inc"

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


helper::LineReader<> reader;
std::string line;

constexpr int MAX_SIZE = 101;
std::array<bool,MAX_SIZE> initial_vect;
std::array<bool,MAX_SIZE> vect;

// TODO: N global
bool try_solve(const int m,const int N) {

	vect = initial_vect;
	
	for (int pos = 1, visited = 1; visited < N - 1; ++visited) {

		for (int i = 0 ; i < m ; ++i) {
			do
			{
				++pos;
				if (pos > N) 
					pos = 2;
			}
			while( vect[pos] );
		}

		if (pos == 13)
			return false;

		vect[pos] = true;
	}
	return true;
}

int inline solve(const int N) {
	for (int i = 2; ; ++i) {
		if (try_solve(i,N))
			return i;
	}
	return -1;
}

int main() {

	initial_vect[0] = true;
	initial_vect[0] = true;

	// TODO: replace std::stol with custom (1-2 digit)
	for (int N = 0 ; reader.get_line(line) && (N=std::stol(line,nullptr,10))!=0 ;) {
		
		int result;
		if (N != 13) {
			result = solve(N);
		}
		else
			result = 1;

		std::cout << result << "\n";
	}
}

