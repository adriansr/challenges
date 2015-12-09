/* solve3.cc / Pattern Matcher Problem / Badoo Challenge 2015
 * author: Adrian Serrano
 * Language: C++11
 *
 * How to build
 * clang++ -std=c++11 solve3.cc -o solve
 * or
 * g++ -std=c++11 solve3.cc -o solve
 */
//#include "helper.1.h"
#include <string>
#include <vector>
#include <iostream>

#define DEBUG( X ) 
class PatternMatcher {
    public:
        PatternMatcher(const std::string& s) {

            size_t pos = 0,
                   limit = s.size();
            
            while(pos<limit) {
                size_t end;
                for (end=pos
                    ;end<limit && TOKENS.find(s[end])==TOKENS.npos
                    ;++end)
                    ;

                if (end != pos) {
                    seq_.push_back( Sequence(Literal,s.substr(pos,end-pos)) );
                }
                pos = end;
                if (pos < limit) {
                    switch (s[pos]) {
                        case '?':
                            seq_.push_back( Sequence(AnySymbol) );
                            ++pos;
                            break;

                        case '^':
                            seq_.push_back( Sequence(AnyUppercase) );
                            ++pos;
                            break;
                        
                        case '*':
                            seq_.push_back( Sequence(Asterisk) );
                            ++pos;
                            break;

                        case '!':
                            for (end=++pos;end<limit && s[end]!='!';++end)
                                ;
                            // TODO: remove
                            if (end == limit)
                                throw "broken input ???";

                            seq_.push_back( Sequence(Exclusion,s.substr(pos,end-pos)) );
                            pos = end + 1;
                            break;
                        
                            // TODO: remove
                        default:
                            throw "broken parser :(";
                    }
                }
            }
            seq_.push_back(End);
            
            DEBUG( std::cerr << "input: '" << s << "'\n" 
                      << "gen: "; 
            for (const auto& e : seq_ ) {
               std::cerr << (int)e.type << "[" << e.data << "] ";
            }
            std::cerr << "\n"; );
        }

        bool matches(const std::string& s) const {
            DEBUG( std::cerr << "match? '" << s << "'\n"; );
            return matches_at(s,0,0);
        }

    private:
        
        static bool literal_match(const std::string& s, size_t pos, size_t len, const std::string& lit) {
            size_t literal_len = lit.size();
            if (len - pos >= literal_len) {
                for (int i = 0; i < literal_len ; ++i) {
                    if (s[pos+i] != lit[i])
                        return false;
                }
                return true;
            }
            return false;
        }

        bool matches_at(const std::string& s, size_t start, size_t expr_pos) const {
            size_t pos = start,
                   len = s.size(),
                   idx = expr_pos;

            while (idx < seq_.size()) {
                const auto& expr = seq_[idx++];
                DEBUG (std::cerr << "matches? " << expr.type << "[" << expr.data << "] : " << s.substr(start) << "\n"; )
                // type switching is bad, but allocations are worse
                switch(expr.type) {
                    case End:
                        if (pos != len)
                            return false;
                    case Literal:
                        if (!literal_match(s,pos,len,expr.data))
                            return false;
                        pos += expr.data.size();
                        break;

                    case AnyUppercase:
                        if (pos==len || s[pos] < 'A' || s[pos]>'Z') {
                            return false;
                        }
                        ++pos;
                        break;

                    case AnySymbol:
                        if (pos == len)
                            return false;
                        ++pos;
                        break;

                    case Exclusion:
                        if (seq_[idx].type == End)
                            return true;
                        if ( s.find(expr.data,pos) != s.npos) {
                            return false;
                        }
                        // fall-back
                    case Asterisk:
                        // quick-win
                        if (seq_[idx].type == End)
                            return true;

                        for(;pos<len;++pos) {
                            if (matches_at(s,pos,idx))
                                return true;
                        }
                        return false;
                }
            }

            return idx == seq_.size();
        }

        const std::string TOKENS = "!*?^";

        enum Type {
            Literal, // sequence of literals "abc"
            Exclusion, // sequence of forbidden text "!abc!"
            AnyUppercase, // single uppercase char ^
            AnySymbol, // single symbol ?
            Asterisk, // zero or more symbols *
            End
        };

        struct Sequence {
            Sequence(Type t) : type(t) {}

            Sequence(Type t, std::string&& s)
                : type(t),data(std::move(s))
            {
            }
            
            Type type;
            std::string data;
        };

        std::vector<Sequence> seq_;
};

#if 0
bool matches(const std::string& expr, const std::string& str) {

    size_t expr_len = expr.size(),
           str_len  = str.size(),
           expr_pos = 0,
           str_pos  = 0;

    while ( expr_pos < expr_len && str_pos < str_len ) {
        char a = expr[expr_pos++],
             b = str[str_pos];

        switch (a) {
            case '^':
                if (b < 'A' || b > 'Z') return false;
                ++str_pos;
                break;

            case '?':
                ++str_pos;
                break;

            case '!':
                while( (a=expr[expr_pos++]) != '!')
        }
    }

    return expr_pos == expr_len && str_pos == str_len;
}
#endif

void fix_windows_crlf(std::string& s) {
    size_t len = s.size();
    if (s[len-1] == '\r') {
        s.resize(len-1);
    }
}
int main() {

    // fast input from stdin
    //helper::LineReader<> reader;
    std::string line;
	
    // fast output to stdout
	//std::string output;
	//output.reserve(4096);
    
    //std::string expr,result ("y\n");

    while ( std::getline(std::cin,line) /*reader.get_line(line)*/ ) {
        
        fix_windows_crlf(line);
        PatternMatcher pm(line);

        //reader.get_line(line);
        std::getline(std::cin,line);
        fix_windows_crlf(line);
        //result[0] = pm.matches(line) ? 'y' : 'n';
        //output.append( result );
        //
        std::cout << (pm.matches(line)? "y\n" : "n\n" );
    }
            
	//write(1,output.data(),output.size());
}

