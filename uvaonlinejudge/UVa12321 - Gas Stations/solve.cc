#include "../../helper/helper.1.h"

struct Station {
    Station(int l, int r) : left(l), right(r) {}
    int left,right;
};

std::vector<Station> v;
int L,G;

int solve() {
    std::sort(v.begin(),v.end(),[](const Station& a, const Station &b) {
                return a.left<b.left || (a.left==b.left && a.right>b.right);
            });
    
    int limit = 0,
        maxpos = 0,
        candidates = 0,
        removed = 0;
    int i,len;
    for (i=0, len=v.size(); i<len; ++i) {
        const auto station = v[i];
        /*std::cerr << "consider " << station.left << "," << station.right 
                  << " lim:" << limit << " mp:" << maxpos 
                  << " rem: " << removed << " can:" << candidates << "\n";*/
        
        if (station.left <= limit) {
            if (station.right>maxpos) {
                maxpos = station.right;
                ++candidates;
                //std::cerr << "select\n";
                if (station.right >= L) {
                    removed += candidates-1;
                    ++i;
                    if (i < len)
                        removed += len - i;
                    //std::cerr << "finish (" << removed << ")\n";
                    break;
                }
            }
            else {
                ++removed;
                //std::cerr << "ignore\n";
            }
        }
        else {
            if (station.left>maxpos) {
                //std::cerr << "gap\n";
                return -1;
            }
            removed += candidates - 1;
            candidates = 0;
            limit = maxpos;
            if (limit >= L) {
                return removed;
            }
            --i;
            //std::cerr << "jump\n";
        }
    }
    return maxpos>=L? removed : -1;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    
    std::array<int,2> arr;
    for (int case_num=0;;++case_num) {

        arr = helper::num_array<int,2>(reader());
        if (arr[0]==0)
            break;
        
        L = arr[0],
        G = arr[1];
        
        if (case_num) {
            v.clear();
        }
        v.reserve(G);
        for (int i = 0; i < G; ++i) {
            arr = helper::num_array<int,2>(reader());
            v.emplace_back( arr[0]-arr[1], arr[0]+arr[1] );
        }

        output.append(solve()).endl();
    }

}

