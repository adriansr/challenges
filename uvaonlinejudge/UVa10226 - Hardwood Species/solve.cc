#include "../../helper/helper.1.h"
#include <map>
#include <cstdio>

helper::LineReader<65536> reader;
std::string line;

char easytolower(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  return in;
} 

int main() {
    std::string output;
    output.reserve(65536);
    
    reader.get_line(line);
    const int N = std::stol(line,nullptr,10);
    reader.get_line(line); // empty line
    
    char buf[16];

    std::map<std::string,int> tree_count;
    std::map<std::string,std::string> tree_name;

    for (int i = 0; i < N; ++i) {
        
        if (i>0) {
            output.push_back('\n');
            tree_count.clear();
            tree_name.clear();
        }

        int count;
        for (count = 0; reader.get_line(line) && !line.empty(); ++count) {
            std::string key;
            std::transform(line.begin(),line.end(),key.begin(),easytolower);
            auto it = tree_name.find(key);
            if (it == tree_name.end()) {
                tree_name[key] = line;
            }
            ++tree_count[key];
        }
        
        for (const auto& key : tree_count) {
            output.append(tree_name[key.first]);
            int len = ::sprintf(&buf[0]," %.04f\n",
                                static_cast<double>(100*key.second)/count);
            output.append(buf,len);
        }
    }
    
    write(1,output.data(),output.size());
}

