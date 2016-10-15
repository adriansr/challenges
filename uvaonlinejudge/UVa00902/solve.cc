//#include "../../helper/helper.1.h"

#ifndef CHALLENGE_HELPER_1_H_INCLUDED
#define CHALLENGE_HELPER_1_H_INCLUDED

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <functional>
#include <bitset>
#include <map>
#include <set>
#include <cstdio>
#include <deque>

using Vi = std::vector<int>;
using ii  = std::pair<int,int>;
using Vii = std::vector<ii>;

namespace helper {

namespace {
bool __attribute__((unused)) IsSpace(char c) {
    return c == ' ';
}

bool __attribute__((unused)) IsNotDigit(char c) {
    return c<'0' || c>'9';
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

        bool operator()(std::string& s) {
            return get_line(s);
        }

        std::string operator()() {
            std::string retval;
            get_line(retval);
            return retval;
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
void for_each_word_predicate(std::string& s, std::function<void(std::string&,size_t,size_t)> pred, size_t start_pos = 0) {
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
void for_each_word(std::string& s, std::function<void(std::string&& word)> pred, size_t start_pos = 0) {
    std::function<void(std::string&,size_t,size_t)> predicate = [&pred](std::string& s, size_t pos, size_t len) { pred( s.substr(pos,len) ); };

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
				if ( write(1,buf_.data(),buf_.size()) != (ssize_t)buf_.size() ) {
					throw "Write to stdout failed";
				}
				buf_.clear();
			}
		}

        BufferedStdout& endl() {
            buf_.push_back('\n');
            return *this;
        }
		
		BufferedStdout& append(const char *s, size_t len) {
			buf_.append(s,len);
			return *this;
		}

		BufferedStdout& append(const char *s) {
			buf_.append(s);
			return *this;
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

        BufferedStdout& append(size_t n) {
			return append( std::to_string(n) );
		}


	private:
		std::string buf_;
};

int inline string_to_int(const std::string& s) {
    return std::stoi(s,nullptr,10);
}

enum BTResult {
    Continue,
    Prune,
    PruneBack,
    Abort,
    Accept
};

namespace {
    template<class Problem,
             class State>
    BTResult do_backtracking(Problem& problem, State state) {
        
        while( problem.next(state) ) {
            BTResult r = problem.eval(state);
            switch(r) {
                case Continue:
                    if ((r=do_backtracking(problem,problem.visit(state))) != Continue) {
                        return r;
                    }
                    break;

                case PruneBack:
                    return Continue;

                case Prune:
                    break;

                case Abort:
                    return Abort;

                case Accept:
                    return Accept;
            }
        }

        return Continue;
    }
}

template<class Problem>
BTResult backtrack(Problem& problem) {
    typename Problem::State root(problem.root());
    auto early_ret = problem.eval(root);
    if (early_ret != Continue) {
        return early_ret;
    }
    return do_backtracking(problem,problem.visit(root));
}

} // namespace helper

#endif // CHALLENGE_HELPER_1_H_INCLUDED
#include <unordered_map>

constexpr size_t MAX_INPUT_LINE_LENGTH = 65536;
using TypeReader = helper::LineReader<MAX_INPUT_LINE_LENGTH>;
static TypeReader reader;

constexpr int SHIFT = 5; // 5 bits per character
constexpr int MAX_DIGITS = 10;

class StringWindow {
public:

    StringWindow(int N, const char *str) 
    : value_(0)
    , mask_( (1L << (SHIFT*N)) - 1) {
        for (int i=0;i<N;++i) {
            value_ = (value_ << SHIFT) | char2index(str[i]);
        }
    }

    int64_t append(char c) {
        value_ = ((value_ << SHIFT) | char2index(c)) & mask_;
        return value_;
    }

    int64_t value() const {
        return value_;
    }

private:
    static int char2index(char c) {
        return c - 'a';
    }

    int64_t value_;
    const int64_t mask_;
};

static std::unordered_map<int64_t, int> substrings;
static char outputVector[MAX_DIGITS+1];

template<class Reader>
class MixedInputReader {
public:
    MixedInputReader(Reader& r): reader_(r), pos_(0) {
    }

    bool next_problem(int *pN, const char **pStart, const char **pEnd) {
        return getN(pN) && getS(pStart, pEnd);
    }

private:

    bool advance() {
        while (pos_ < line_.size() && line_[pos_] == ' ') ++pos_;
        while (pos_ >= line_.size()) {
            pos_ = 0;
            bool result = reader_.get_line(line_);
            if (!result) return false;
            while (line_[pos_] == ' ') ++pos_;
        }
        return true;
    }

    bool getN(int *pN) {
        bool rv = advance();
        if (rv) {
            int val = line_[pos_++] - '0';
            if (val == 1 && pos_ < line_.size() && line_[pos_]=='0') {
                val = 10;
                ++ pos_;
            }
            *pN = val;
        }
        return rv;
    }

    bool getS(const char **pStart, const char **pEnd) {
        bool rv = advance();
        if (rv) {
            *pStart = line_.data() + pos_;
            while (pos_ < line_.size() && line_[pos_] != ' ') ++pos_;
            *pEnd = line_.data() + pos_;
        }
        return rv;
    }

    Reader& reader_;
    std::string line_;
    size_t pos_;
};

int main() {
    helper::BufferedStdout output;
    
    MixedInputReader<TypeReader> mixedReader(reader);

    int N;
    const char *str, *end;
    while (mixedReader.next_problem(&N, &str, &end)) {
        
        //std::cout << "n: " << N << '\n';

        StringWindow acc(N, str);
        int maxCount = 1;
        int64_t maxValue = acc.value();
        
        //std::cout << std::hex << "start: " << maxValue << "\n";
        substrings.clear();
        substrings[maxValue] = 1;
        for (str += N; str < end; ++str) {
            auto value = acc.append(*str);
            int candidate = ++substrings[value];
            if (candidate > maxCount) {
                maxCount = candidate;
                maxValue = value;
                //std::cout << std::hex << "got: " << maxValue << " " << maxCount << "\n";
            }
        }
        for (int shift=(N-1)*SHIFT,i=0;i<N;shift-=SHIFT,++i) {
            outputVector[i] = 'a' + ((maxValue >> shift)&((1<<SHIFT))-1);
        }
        outputVector[N] = '\n';
        output.append(outputVector, N+1);
    }
}

