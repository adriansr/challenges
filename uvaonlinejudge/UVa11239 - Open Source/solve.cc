#include "../../helper/helper.1.h"
#include <map>
#include <set>

#define CONTAINS(SET,ITEM) (SET.find(ITEM)!=SET.cend())

struct Project {
    Project(const std::string& title)
        : title(title) {}

    std::string title;
    std::set<std::string> students;

    size_t size() const {
        return students.size();
    }

    bool operator<(const Project& other) const {
        return size() > other.size() || (size()==other.size() && title<other.title);
    }
};

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    std::vector<Project> data;
    std::set<std::string> banned;

    for(int ncase=0;;++ncase) {
        if (ncase) {
            data.clear();
            banned.clear();
        }
        int current = -1;
        
        for (;;) {
            reader.get_line(line);
            if (line[0]>='A' && line[0]<='Z') {
                data.emplace_back(line);
                ++current;
            }
            else if (line[0]=='0') {
                return 0;
            }
            else if (line[0]=='1') {
                break;
            }
            else {
                if (!CONTAINS(banned,line)) {
                    bool repeated = false;
                    for (int i=0;i<current;++i) {
                        auto& p = data[i].students;
                        auto it = p.find(line);
                        if (it != p.cend()) {
                            repeated = true;
                            p.erase(it);
                        }
                    }
                    if (!repeated) {
                        data[current].students.insert(line);
                    }
                    else {
                        banned.insert(line);
                    }
                }
            }
        }

        std::sort(data.begin(),data.end());

        for (const auto& project : data) {
            output.append(project.title).append(' ').append(project.size()).endl();
        }
    }
}

