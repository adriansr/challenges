#include "../../helper/helper.1.h"
#include <queue>

constexpr int MAX_N = 10;

bool is_solution(const Vi& p) {
    for (int i=0;i<p.size();++i) 
        if (p[i]!=1+i)
            return false;
    return true;
}

int max_distance_to_solution(const Vi& state) {
    int num_wrong=0;
    for (int i=0;i<state.size();++i) {
        if (state[i] != i+1)
            num_wrong++;
    }
    return num_wrong;
}

Vi mk_solution(int N) {
    Vi result;
    result.resize(N);
    for (int i=0;i<N;++i) 
        result[i] = i + 1;
    return result;
}

using DistMap = std::map<Vi,int>;

bool relax(const Vi& state,
           int distance,
           DistMap& dist) {

    auto it = dist.find(state);
    if (it != dist.end() && it->second <= distance) {
        return false;
    }

    dist[state] = distance;
    return true;
}

std::pair<Vi,Vi> mk_cut(const Vi& source, int N, int start, int end) {
    Vi rest, cut;
    int to_cut = end - start + 1;
    rest.resize(N-to_cut);
    cut.resize(to_cut);
    for (int i=0;i<start;++i) {
        rest[i] = source[i];
    }
    for (int i=start;i<=end;++i) {
        cut[i-start] = source[i];
    }
    for (int i=end+1;i<N;++i) {
        rest[i-end-1+start] = source[i];
    }
    return std::make_pair(rest,cut);
}

void mk_paste(Vi& dest, const Vi& rest, const Vi& cut, const int pos) {
    for (int i=0;i<pos;++i) {
        dest[i] = rest[i];
    }
    for (int j=0;j<cut.size();++j) {
        dest[pos+j] = cut[j];
    }
    for (int i=pos;i<rest.size();++i) {
        dest[i+cut.size()] = rest[i];
    }
}

/*
bool validate(const Vi& state, const Vi& source) {
    if (state.size() == source.size()) {
        if (state != source) {
            std::array<int,MAX_N> count = {0};
            for (int val : state) {
                count[val] ++;
            }
            for (int i=1;i<=state.size();++i) {
                if (count[i]!=1)
                    return false;
            }
            return true;
        }
    }
    return false;
}*/

void bfs_limit(const Vi& start,
               DistMap& dist,
               int max_depth) {

    int N = start.size();
    std::queue<std::pair<Vi,int>> q;

    q.emplace(start,0);
    
    Vi state,
       new_state(start),
       rest,
       cut;
    int distance;
    int count = 0,
        max_start = N;
    do {
        ++count;
        std::tie(state,distance) = q.front();
        q.pop();
        
        if (relax(state,distance,dist) && distance < max_depth) {
            
            int new_distance = distance + 1;
            for (int start = 0; start < max_start; ++start) {
                for (int end=start;end<N;++end) {
                    if (end-start+1 <= N/2) {
                    std::tie(rest,cut) = mk_cut(state,N,start,end);
                    
                    for (int dest = 0; dest<=rest.size(); ++dest) {
                        if (dest != start) {
                            mk_paste(new_state,rest,cut,dest);
                            q.emplace(new_state,new_distance);
                            /*if (!validate(new_state,state)) {
                                std::cerr << "< start:"<<start<<" end:"<<end<<"\n";
                                helper::report("< source",state);
                                helper::report("< rest",rest);
                                helper::report("< cut",cut);
                                std::cerr << "> at dest:"<<dest<<"\n";
                                helper::report("> new",new_state);
                            }*/
                        }
                    }
                    }
                }
            }
        }
    }
    while (!q.empty());
    //std::cerr << "BFS count:" << count << '\n';
}

int solve(int N, Vi& source) {

    if (is_solution(source))
        return 0;

    DistMap dist_source,
            dist_solution;

    Vi solution(mk_solution(N));
    int max_depth = std::min(max_distance_to_solution(source),5),
        first_depth = (max_depth-1) / 2,
        second_depth = max_depth - first_depth - 1;

    //std::cerr << "MaxD:"<<max_depth << " 1:" << first_depth << " 2:" << second_depth << '\n';
    bfs_limit(source,dist_source,first_depth);
    auto it = dist_source.find(solution);
    if (it != dist_source.end()) {
        std::cerr << "Found at first\n";
        return it->second;
    }

    bfs_limit(solution,dist_solution,second_depth);
    /*it = dist_solution.find(source);
    if (it != dist_solution.end()) {
        return it->second;
    }*/

    int min_val = max_depth;

    const auto& dist_a = first_depth < second_depth? dist_source   : dist_solution;
    const auto& dist_b = first_depth < second_depth? dist_solution : dist_source;

    for (const auto it : dist_a) {
        const auto it2 = dist_b.find(it.first);
        if (it2 != dist_source.end()) {
            int val = it.second + it2->second;
           // std::cerr << "found " << it.second << " + " << it2->second << " = " << val << '\n';
            if (val < min_val) 
                min_val = val;
        }
    }
    std::cerr << "Found at second\n";
    return min_val;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
    
    for (int ncase=0;reader.get_line(line);++ncase) {

        const int num_paragraphs = helper::string_to_int(line);
        if (num_paragraphs == 0)
            break;

        Vi p;
        p.reserve(num_paragraphs);
        p = helper::num_vector<int>(reader());
        
        int result = solve(num_paragraphs,p);
        output.append("Case ",5).append(ncase+1).append(": ",2).append(result).endl();
    }
}

