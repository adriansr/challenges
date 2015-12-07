//#include "../../helper/helper.1.inc"

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <functional>

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
void split_line(const std::string &s, Container *pcontainer, size_t pos = 0) {
	
	Inserter inserter(pcontainer);

	size_t limit = s.size();

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

std::array<FieldType,FixedLength> num_array(const std::string& s, size_t pos = 0) {
	
	std::array<FieldType,FixedLength> retval{};
	using Container = decltype(retval);
	split_line<FieldType,Container,FixedSizeInserter<Container>,Convert,IsDelimiter>(s,&retval,pos);
	//report("num_array",retval);
	return retval;
}

template<typename FieldType,
		 FieldType (*Convert)(const std::string&) = & ConvertStr<FieldType>,
		 bool (*IsDelimiter)(char) = IsSpace >

std::vector<FieldType> num_vector(const std::string& s, size_t pos = 0) {
	
	std::vector<FieldType> retval;
	using Container = decltype(retval);
	split_line<FieldType,Container,VariableSizeInserter<Container>,Convert,IsDelimiter>(s,&retval,pos);
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
			ssize_t ret = ::read(0,&buffer_[remain],fit);
			if (ret == static_cast<ssize_t>(fit)) {
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

template<
	bool (*IsDelimiter)(char) = IsSpace >
void for_each_word(const std::string& s, std::function<void(const std::string& word)> pred) {
	size_t pos = 0,
		   len = s.size();
	
	do {

		for (;pos<len && IsDelimiter(s[pos]);++pos)
			;

		size_t end;
		for (end=pos;end<len && !IsDelimiter(s[end]);++end)
			;

		if (pos==end)
			break;

		pred( s.substr(pos,end-pos) );
		pos = end;
	}
	while (pos < len);
}

} // namespace helper
#include <set>

helper::LineReader<> reader;
std::string line;


int main() {
	
	std::string output;
	output.reserve(4096);
	
	for (int case_num=1;;++case_num) {

		reader.get_line(line);
		const int N = std::stol(line,nullptr,10);
		if (N == 0) 
			break;
		
		output.append("Case ",5);
		output.append(std::to_string(case_num));
		output.append(":\n",2);

		std::set<int> sums;
		
		std::vector<int> nums;
		nums.resize(N*(N-1)); 
		for (int i=0 ; i<N ; ++i) {
			reader.get_line(line);
			int ni = std::stol(line,nullptr,10);
			
			nums[i] = ni;
			//sums.insert(ni);

			for (int j=0;j<i;++j) {
				sums.insert( ni + nums[j] );
			}
		}

		//sums.insert(nums.cbegin(),nums.cend());

		reader.get_line(line);
		const int M = std::stol(line,nullptr,10);
		
		for (int i=0 ; i<M ; ++i) {
			reader.get_line(line);
			int mi = std::stol(line,nullptr,10);

			auto range = sums.equal_range(mi);

			if (range.first != sums.begin()) {
				--range.first;
			}
			if (range.second != sums.end()) {
				++range.second;
			}

			int lower_val = *range.first,
				lower_diff = lower_val > mi ? lower_val - mi : mi - lower_val;
			
			for (auto it = range.first ; ++it != range.second; ) {
				int val = *it,
					diff = val > mi? val - mi : mi - val;
				if (diff < lower_diff) {
					lower_diff = diff;
					lower_val  = val;
				}
				else 
					break;
			}

			output.append("Closest sum to ");
			output.append(std::to_string(mi));
			output.append(" is ");
			output.append(std::to_string(lower_val));
			output.append(".\n",2);

		}

	}

	write(1,output.data(),output.size());
}

