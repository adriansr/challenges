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

} // namespace helper

#include <cstring>

helper::LineReader<4096> reader;
std::string line;

class Stack {
    bool eval(int value, int index, int prev_value, int prev_index) {
    }
};

template<class Impl>
class Detector {
public:
    enum {
        Impossible = -1
    };

    int confidence() const {
        return m_confidence!=Impossible? m_confidence : 0;
    }
    
    void on_value( int value, int index, int prev_value, int prev_index) {
        sample(MImpl.eval(value,index,prev_value,prev_index));
    }

private:
    void sample(bool result) {
        if (result) {
            if (m_confidence!=Impossible) m_confidence ++ ;
        }
        else {
            m_confidence = Impossible;
        }
    }

    int m_confidence = 0;
};

#define MAX_VALUE 101
struct State {
    int pos[MAX_VALUE];

    int stack_level,
        queue_level,
        prioq_level,
        impossible;
} empty, state;

int main() {
    
    std::string output;
    output.reserve(4096);

    line.reserve(8);

    for (int count=0;;++count) {
        if(!reader.get_line(line))
            break;
        
        const int N = std::stol(line,nullptr,10);
        
        if (count) {
            state = empty;
        }

        int prev = -1,
            prev_idx = -1;
        
        for (int i=1,icount=0;i<=N;++i) {
            reader.get_line(line);

            int num = 0;
            for (int j=2,len=line.size();j<len;++j) {
                num = num * 10 + line[j] - '0';
            }

            if (line[0] == '1') {
                state.pos[num] = ++icount;
                std::cerr << "insert " << num << " as " << icount << "\n";
            }
            else {
                int cur_idx  = state.pos[num];
                
                std::cerr << "get " << num << " as " << cur_idx << " prev " << prev << " as " << prev_idx << "\n";

                if (cur_idx == 0) {
                    state.impossible ++;
                }

                if (prev != -1) {
                    bool hit = false;
                    if (prev_idx +1 == cur_idx) {
                        state.queue_level ++;
                        hit = true;
                    }
                    else if (prev_idx == cur_idx + 1) {
                        state.stack_level ++;
                        hit = true;
                    }
                    if (prev > num) {
                        state.prioq_level ++;
                        hit = true;
                    }
                    if (!hit) {
                        state.impossible ++;
                    }
                }
                prev = num;
                prev_idx = cur_idx;;
            }
        }

        std::cout << "stack: " << state.stack_level << "\n"
                  << "queue: " << state.queue_level << "\n"
                  << "prioq: " << state.prioq_level << "\n"
                  << "impos: " << state.impossible << "\n";

        if (state.impossible) {
            output.append("impossible\n",11);
        }
        else {
            if (state.stack_level > state.queue_level
             && state.stack_level > state.prioq_level) {
                output.append("stack\n",6);
            }
            else if (state.queue_level > state.stack_level
                  && state.queue_level > state.prioq_level) {
                output.append("queue\n",6);
            }
            else if (state.prioq_level > state.stack_level
                  && state.prioq_level > state.queue_level) {
                output.append("priority queue\n",15);
            }
            else {
                output.append("not sure\n",9);
            }
        }
    }

    write(1,output.data(),output.size());
}

