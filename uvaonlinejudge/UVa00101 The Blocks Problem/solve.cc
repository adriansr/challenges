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
        bool       ok_;
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

// TODO: crashes when line > BUFFER_SIZE
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
	class Predicate,
    bool (*IsDelimiter)(char) = IsSpace >
void for_each_word_predicate(std::string& s, Predicate pred, size_t start_pos = 0) {
    size_t pos = start_pos,
           len = s.size();
    
    do {

        for (;pos<len && IsDelimiter(s[pos]);++pos)
            ;

        size_t end;
        for (end=pos;end<len && !IsDelimiter(s[end]);++end)
            ;

        if (pos==end)
            break;
		
		pred(s, pos, end - pos);
        pos = end;
    }
    while (pos < len);
}

template<
    bool (*IsDelimiter)(char) = IsSpace >
void for_each_word(const std::string& s, std::function<void(std::string&& word)> pred, size_t start_pos = 0) {
	auto predicate = [&pred](std::string& s, size_t pos, size_t len) { pred( s.substr(pos,len) ); };

	for_each_word_predicate<IsDelimiter>(s,predicate,start_pos);
}
/*
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
*/

template<
    typename IntType = int,
    bool (*IsDelimiter)(char) = IsSpace >
void for_each_number(const std::string& s, std::function<void(IntType)> pred) {
    size_t pos = 0,
           len = s.size();
    
    do {

        for (;pos<len && IsDelimiter(s[pos]);++pos)
            ;
        
        IntType num = 0;   
        size_t end;
        for (end=pos;end<len && !IsDelimiter(s[end]);++end) {
            num = num * 10 + s[end] - '0';
        }

        if (pos==end)
            break;

        pred( num );
        pos = end;
    }
    while (pos < len);
}

template<typename IntType = int>
IntType extract_positive_number(const std::string& s, size_t start_pos = 0, size_t* end_pos = nullptr){
	IntType num;
	size_t pos;
	char c;
	for (num = 0 , pos = start_pos ; (c=s[pos])>='0' && c <= '9' ; ++pos ) {
		num = num*10 + c - '0';
	}
	if (end_pos!=nullptr) {
		*end_pos = pos;
	}
	return num;
}

class BufferedStdout {
	public:
		BufferedStdout(size_t reserve = 4096) {
			buf_.reserve(reserve);
		}
		
		~BufferedStdout() {
			flush();
		}

		void flush() {
			if (!buf_.empty()) {
				if ( write(1,buf_.data(),buf_.size()) != buf_.size() ) {
					throw "Write to stdout failed";
				}
				buf_.clear();
			}
		}
		
		BufferedStdout& append(const std::string& s) {
			buf_.append(s);
			return *this;
		}
		
		BufferedStdout& append(char c) {
			buf_.push_back(c);
			return *this;
		}
	
		BufferedStdout& append(int n) {
			return append( std::to_string(n) );
		}

	private:
		std::string buf_;
};

} // namespace helper
#include <map>
#include <list>

class RobotArm {
	public:
		typedef int Block;
		typedef int Position;
		using Stack = std::list<Block>;

		RobotArm(int num_blocks) : N_(num_blocks) {
			stacks_.reserve(N_);
			for (int i = 0 ; i < N_ ; ++i) {
				stacks_.push_back( { i } );
				location_[i] = &stacks_.back();
			}
		}
		
		bool parse_cmd(const std::string& line);
		
		void move_onto(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			expose(src_stack,a);
			expose(dst_stack,b);
			src_stack->pop_back();
			dst_stack->push_back(a);
			location_[a] = dst_stack;
		}
		
		void move_over(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			expose(src_stack,a);
			src_stack->pop_back();
			dst_stack->push_back(a);
			location_[a] = dst_stack;
		}
		
		void pile_onto(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;
			
			expose(dst_stack,b);

			auto   end_it = src_stack->end(),
				 start_it = std::find( src_stack->begin(), end_it, a);

			std::for_each(start_it,end_it,
						  [this,dst_stack](Block val) { location_[val] = dst_stack; } );

			dst_stack->splice(dst_stack->end(), *src_stack, start_it, end_it);
		}
		
		void pile_over(Block a, Block b) {
			if (a == b) return;

			Stack *src_stack = location_[a],
				  *dst_stack = location_[b];
			
			if (src_stack == dst_stack)
				return;

			auto   end_it = src_stack->end(),
				 start_it = std::find( src_stack->begin(), end_it, a);

			std::for_each(start_it,end_it,
						  [this,dst_stack](Block val) { location_[val] = dst_stack; } );
			
			dst_stack->splice(dst_stack->end(), *src_stack, start_it, end_it);
		}

		const Stack& stack(Position pos) const {
			return stacks_[pos];
		}

	private:
		void expose(Stack *stack, Block b) {
			Block a;
			while ( (a=stack->back()) != b) {
				stack->pop_back();
				move_to_initial(a);
			}
		}

		void move_to_initial(Block b) {
			Stack *dst_stack = &stacks_[b];

			if (!dst_stack->empty()) {
				throw "this should never happen";
			}
			
			dst_stack->push_back(b);
			location_[b] = dst_stack;
		}

		// given a block, remove it from wherever it is
		// doesn't update it's location 
		void remove_block(Block b) {
			location_[b]->remove(b);
		}

		const int N_;
		std::vector<Stack> stacks_;
		std::map<Block,Stack*> location_;
};

bool RobotArm::parse_cmd(const std::string& line) {
	if (line[0] == 'q')
		return false;
	
	int opcode = line[0]=='m'? 0 : 2;

	size_t pos;
	int a = helper::extract_positive_number(line,5,&pos);
	
	if (line[pos+2] == 'n' )
		opcode |= 1;

	int b = helper::extract_positive_number(line,pos+6,nullptr);

	switch (opcode) {
	case 0: // move over
		move_over(a,b);
		break;

	case 1: // move onto
		move_onto(a,b);
		break;

	case 2: // pile over
		pile_over(a,b);
		break;

	case 3: // pile onto
		pile_onto(a,b);
		break;
	}

	return true;
}

int main() {
	helper::LineReader<> reader;
	std::string line;

	helper::BufferedStdout output;

	reader.get_line(line);
	const int N = std::stoi(line,nullptr,10);

	RobotArm robot(N);
	
	while ( reader.get_line(line) && robot.parse_cmd(line) )
		;
	for (int i = 0; i < N ; ++i ) {
		output.append(i).append(':');
		const auto& stack = robot.stack(i);
		for (auto elem : stack) {
			output.append(' ').append(elem);
		}
		output.append('\n');
	}
}

