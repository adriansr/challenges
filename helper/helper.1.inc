#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

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
	
	assert( inserter.ok() );
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
