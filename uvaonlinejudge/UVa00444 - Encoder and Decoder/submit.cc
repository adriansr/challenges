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
constexpr size_t MAX_INPUT_LINE_LENGTH = 16384;
static helper::LineReader<MAX_INPUT_LINE_LENGTH> reader;

// autogenerated by gen_encoder.py
char encoder_table[128 * 4] = {
   '0', '0', '0',  0, // 0
   '0', '0', '0',  0, // 1
   '0', '0', '0',  0, // 2
   '0', '0', '0',  0, // 3
   '0', '0', '0',  0, // 4
   '0', '0', '0',  0, // 5
   '0', '0', '0',  0, // 6
   '0', '0', '0',  0, // 7
   '0', '0', '0',  0, // 8
   '0', '0', '0',  0, // 9
   '0', '0', '0',  0, // 10
   '0', '0', '0',  0, // 11
   '0', '0', '0',  0, // 12
   '0', '0', '0',  0, // 13
   '0', '0', '0',  0, // 14
   '0', '0', '0',  0, // 15
   '0', '0', '0',  0, // 16
   '0', '0', '0',  0, // 17
   '0', '0', '0',  0, // 18
   '0', '0', '0',  0, // 19
   '0', '0', '0',  0, // 20
   '0', '0', '0',  0, // 21
   '0', '0', '0',  0, // 22
   '0', '0', '0',  0, // 23
   '0', '0', '0',  0, // 24
   '0', '0', '0',  0, // 25
   '0', '0', '0',  0, // 26
   '0', '0', '0',  0, // 27
   '0', '0', '0',  0, // 28
   '0', '0', '0',  0, // 29
   '0', '0', '0',  0, // 30
   '0', '0', '0',  0, // 31
   '2', '3', '0',  2, // 32
   '3', '3', '0',  2, // 33
   '0', '0', '0',  0, // 34
   '0', '0', '0',  0, // 35
   '0', '0', '0',  0, // 36
   '0', '0', '0',  0, // 37
   '0', '0', '0',  0, // 38
   '0', '0', '0',  0, // 39
   '0', '0', '0',  0, // 40
   '0', '0', '0',  0, // 41
   '0', '0', '0',  0, // 42
   '0', '0', '0',  0, // 43
   '4', '4', '0',  2, // 44
   '0', '0', '0',  0, // 45
   '6', '4', '0',  2, // 46
   '0', '0', '0',  0, // 47
   '0', '0', '0',  0, // 48
   '0', '0', '0',  0, // 49
   '0', '0', '0',  0, // 50
   '0', '0', '0',  0, // 51
   '0', '0', '0',  0, // 52
   '0', '0', '0',  0, // 53
   '0', '0', '0',  0, // 54
   '0', '0', '0',  0, // 55
   '0', '0', '0',  0, // 56
   '0', '0', '0',  0, // 57
   '8', '5', '0',  2, // 58
   '9', '5', '0',  2, // 59
   '0', '0', '0',  0, // 60
   '0', '0', '0',  0, // 61
   '0', '0', '0',  0, // 62
   '3', '6', '0',  2, // 63
   '0', '0', '0',  0, // 64
   '5', '6', '0',  2, // 65
   '6', '6', '0',  2, // 66
   '7', '6', '0',  2, // 67
   '8', '6', '0',  2, // 68
   '9', '6', '0',  2, // 69
   '0', '7', '0',  2, // 70
   '1', '7', '0',  2, // 71
   '2', '7', '0',  2, // 72
   '3', '7', '0',  2, // 73
   '4', '7', '0',  2, // 74
   '5', '7', '0',  2, // 75
   '6', '7', '0',  2, // 76
   '7', '7', '0',  2, // 77
   '8', '7', '0',  2, // 78
   '9', '7', '0',  2, // 79
   '0', '8', '0',  2, // 80
   '1', '8', '0',  2, // 81
   '2', '8', '0',  2, // 82
   '3', '8', '0',  2, // 83
   '4', '8', '0',  2, // 84
   '5', '8', '0',  2, // 85
   '6', '8', '0',  2, // 86
   '7', '8', '0',  2, // 87
   '8', '8', '0',  2, // 88
   '9', '8', '0',  2, // 89
   '0', '9', '0',  2, // 90
   '0', '0', '0',  0, // 91
   '0', '0', '0',  0, // 92
   '0', '0', '0',  0, // 93
   '0', '0', '0',  0, // 94
   '0', '0', '0',  0, // 95
   '0', '0', '0',  0, // 96
   '7', '9', '0',  2, // 97
   '8', '9', '0',  2, // 98
   '9', '9', '0',  2, // 99
   '0', '0', '1',  3, // 100
   '1', '0', '1',  3, // 101
   '2', '0', '1',  3, // 102
   '3', '0', '1',  3, // 103
   '4', '0', '1',  3, // 104
   '5', '0', '1',  3, // 105
   '6', '0', '1',  3, // 106
   '7', '0', '1',  3, // 107
   '8', '0', '1',  3, // 108
   '9', '0', '1',  3, // 109
   '0', '1', '1',  3, // 110
   '1', '1', '1',  3, // 111
   '2', '1', '1',  3, // 112
   '3', '1', '1',  3, // 113
   '4', '1', '1',  3, // 114
   '5', '1', '1',  3, // 115
   '6', '1', '1',  3, // 116
   '7', '1', '1',  3, // 117
   '8', '1', '1',  3, // 118
   '9', '1', '1',  3, // 119
   '0', '2', '1',  3, // 120
   '1', '2', '1',  3, // 121
   '2', '2', '1',  3, // 122
   '0', '0', '0',  0, // 123
   '0', '0', '0',  0, // 124
   '0', '0', '0',  0, // 125
   '0', '0', '0',  0, // 126
   '0', '0', '0',  0, // 127
};
// end of autogenerated

// autogenerated by gen_trie.py
#define INNER_NODE(ARGS...) 0, ARGS, 0
#define LEAF_NODE(VALUE) 1, VALUE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
static char decoder_trie[93*12] = {
   /* node[ 0]: */ INNER_NODE(21,23,1,3,5,14,7,17,9,11),
   /* node[ 1]: */ INNER_NODE(51,71,91,2,0,0,0,25,35,0),
   /* node[ 2]: */ LEAF_NODE(' '),
   /* node[ 3]: */ INNER_NODE(53,73,0,4,0,0,13,26,36,0),
   /* node[ 4]: */ LEAF_NODE('!'),
   /* node[ 5]: */ INNER_NODE(55,75,0,0,6,0,0,27,37,0),
   /* node[ 6]: */ LEAF_NODE(','),
   /* node[ 7]: */ INNER_NODE(59,79,0,0,8,0,16,29,39,0),
   /* node[ 8]: */ LEAF_NODE('.'),
   /* node[ 9]: */ INNER_NODE(63,83,0,0,0,10,19,31,41,45),
   /* node[10]: */ LEAF_NODE(':'),
   /* node[11]: */ INNER_NODE(65,85,0,0,0,12,20,32,42,46),
   /* node[12]: */ LEAF_NODE(';'),
   /* node[13]: */ LEAF_NODE('?'),
   /* node[14]: */ INNER_NODE(57,77,0,0,0,0,15,28,38,0),
   /* node[15]: */ LEAF_NODE('A'),
   /* node[16]: */ LEAF_NODE('B'),
   /* node[17]: */ INNER_NODE(61,81,0,0,0,0,18,30,40,44),
   /* node[18]: */ LEAF_NODE('C'),
   /* node[19]: */ LEAF_NODE('D'),
   /* node[20]: */ LEAF_NODE('E'),
   /* node[21]: */ INNER_NODE(47,67,87,0,0,0,0,22,33,43),
   /* node[22]: */ LEAF_NODE('F'),
   /* node[23]: */ INNER_NODE(49,69,89,0,0,0,0,24,34,0),
   /* node[24]: */ LEAF_NODE('G'),
   /* node[25]: */ LEAF_NODE('H'),
   /* node[26]: */ LEAF_NODE('I'),
   /* node[27]: */ LEAF_NODE('J'),
   /* node[28]: */ LEAF_NODE('K'),
   /* node[29]: */ LEAF_NODE('L'),
   /* node[30]: */ LEAF_NODE('M'),
   /* node[31]: */ LEAF_NODE('N'),
   /* node[32]: */ LEAF_NODE('O'),
   /* node[33]: */ LEAF_NODE('P'),
   /* node[34]: */ LEAF_NODE('Q'),
   /* node[35]: */ LEAF_NODE('R'),
   /* node[36]: */ LEAF_NODE('S'),
   /* node[37]: */ LEAF_NODE('T'),
   /* node[38]: */ LEAF_NODE('U'),
   /* node[39]: */ LEAF_NODE('V'),
   /* node[40]: */ LEAF_NODE('W'),
   /* node[41]: */ LEAF_NODE('X'),
   /* node[42]: */ LEAF_NODE('Y'),
   /* node[43]: */ LEAF_NODE('Z'),
   /* node[44]: */ LEAF_NODE('a'),
   /* node[45]: */ LEAF_NODE('b'),
   /* node[46]: */ LEAF_NODE('c'),
   /* node[47]: */ INNER_NODE(0,48,0,0,0,0,0,0,0,0),
   /* node[48]: */ LEAF_NODE('d'),
   /* node[49]: */ INNER_NODE(0,50,0,0,0,0,0,0,0,0),
   /* node[50]: */ LEAF_NODE('e'),
   /* node[51]: */ INNER_NODE(0,52,0,0,0,0,0,0,0,0),
   /* node[52]: */ LEAF_NODE('f'),
   /* node[53]: */ INNER_NODE(0,54,0,0,0,0,0,0,0,0),
   /* node[54]: */ LEAF_NODE('g'),
   /* node[55]: */ INNER_NODE(0,56,0,0,0,0,0,0,0,0),
   /* node[56]: */ LEAF_NODE('h'),
   /* node[57]: */ INNER_NODE(0,58,0,0,0,0,0,0,0,0),
   /* node[58]: */ LEAF_NODE('i'),
   /* node[59]: */ INNER_NODE(0,60,0,0,0,0,0,0,0,0),
   /* node[60]: */ LEAF_NODE('j'),
   /* node[61]: */ INNER_NODE(0,62,0,0,0,0,0,0,0,0),
   /* node[62]: */ LEAF_NODE('k'),
   /* node[63]: */ INNER_NODE(0,64,0,0,0,0,0,0,0,0),
   /* node[64]: */ LEAF_NODE('l'),
   /* node[65]: */ INNER_NODE(0,66,0,0,0,0,0,0,0,0),
   /* node[66]: */ LEAF_NODE('m'),
   /* node[67]: */ INNER_NODE(0,68,0,0,0,0,0,0,0,0),
   /* node[68]: */ LEAF_NODE('n'),
   /* node[69]: */ INNER_NODE(0,70,0,0,0,0,0,0,0,0),
   /* node[70]: */ LEAF_NODE('o'),
   /* node[71]: */ INNER_NODE(0,72,0,0,0,0,0,0,0,0),
   /* node[72]: */ LEAF_NODE('p'),
   /* node[73]: */ INNER_NODE(0,74,0,0,0,0,0,0,0,0),
   /* node[74]: */ LEAF_NODE('q'),
   /* node[75]: */ INNER_NODE(0,76,0,0,0,0,0,0,0,0),
   /* node[76]: */ LEAF_NODE('r'),
   /* node[77]: */ INNER_NODE(0,78,0,0,0,0,0,0,0,0),
   /* node[78]: */ LEAF_NODE('s'),
   /* node[79]: */ INNER_NODE(0,80,0,0,0,0,0,0,0,0),
   /* node[80]: */ LEAF_NODE('t'),
   /* node[81]: */ INNER_NODE(0,82,0,0,0,0,0,0,0,0),
   /* node[82]: */ LEAF_NODE('u'),
   /* node[83]: */ INNER_NODE(0,84,0,0,0,0,0,0,0,0),
   /* node[84]: */ LEAF_NODE('v'),
   /* node[85]: */ INNER_NODE(0,86,0,0,0,0,0,0,0,0),
   /* node[86]: */ LEAF_NODE('w'),
   /* node[87]: */ INNER_NODE(0,88,0,0,0,0,0,0,0,0),
   /* node[88]: */ LEAF_NODE('x'),
   /* node[89]: */ INNER_NODE(0,90,0,0,0,0,0,0,0,0),
   /* node[90]: */ LEAF_NODE('y'),
   /* node[91]: */ INNER_NODE(0,92,0,0,0,0,0,0,0,0),
   /* node[92]: */ LEAF_NODE('z'),
};
// end of autogenerated

static const char * const root = decoder_trie;
constexpr int MAX_LINE_LEN = 80;
static char reverse[MAX_LINE_LEN];

int main() {
    helper::BufferedStdout output;
    std::string line;
    
    while (reader.get_line(line)) {

        if ((line[0] & 0xF0) != '0') {
            for (int i = line.size(); i>0;) {
                char c = line[--i];
                if (c > 0) {
                    const char *ptr = encoder_table + (c << 2);
                    output.append(ptr, ptr[3]);
                }
            }
            output.endl();
        } else {
            int dest = MAX_LINE_LEN;
            const char *node = root;
            for(char c: line) {
                node = &decoder_trie[node[1 + c - '0'] * 12];
                if (node[0]) {
                    reverse[--dest] = node[1];
                    node = root;
                }
            }
            output.append(&reverse[dest], MAX_LINE_LEN - dest).endl();
        }
    }
}

