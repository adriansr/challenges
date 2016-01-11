#include "../../helper/helper.1.h"
#include <queue>

constexpr int MAX_DIM = 75 + 2,
              LIMIT = 5500,
              INF = 1000000;
int W,
    H;

using RegionId = int;
constexpr int NO_REGION = 0,
              MAX_ADJ_REGIONS = 4;

enum Direction {
    Up = 0,
    Down,
    Left,
    Right
};

const std::array<ii,MAX_ADJ_REGIONS> dir_delta {
    std::make_pair(0,-1),
    std::make_pair(0,1),
    std::make_pair(-1,0),
    std::make_pair(1,0)
};

struct Item {
    bool is_piece;
    std::array<RegionId,MAX_ADJ_REGIONS> region;

    void reset() {
        is_piece = false;
        region.fill(NO_REGION);
    }
};

std::array<std::array<Item,MAX_DIM>,MAX_DIM> board;
std::array<std::bitset<MAX_DIM>,MAX_DIM> visited;

bool coords_valid(int x, int y) {
    return x>=0 && x<W && y>=0 && y<H;
}

std::vector<RegionId> shared_regions(const Item& a, const Item& b) {
    std::vector<RegionId> result;
    if (a.is_piece && b.is_piece) {
        for (int region_a : a.region) {
            if (region_a != NO_REGION) {
                for (int region_b : b.region) {
                    if (region_a == region_b) {
                        result.push_back(region_a);
                        break;
                    }
                }
            }
        }
    }
    else if (a.is_piece || b.is_piece) {
        const Item& p = a.is_piece? a : b,
                    s = a.is_piece? b : a;
        int region = s.region[0];
        for (int region_p : p.region) {
            if (region_p == region) {
                result.push_back(region);
                break;
            }
        }
    }
    else {
        if (a.region[0] == b.region[0]) {
            result.push_back(a.region[0]);
        }
    }

    return result;
}

void flood_fill(int start_x, int start_y, RegionId color) {

    std::queue<ii> q;
    q.emplace(start_x,start_y);

    int count = 0;

    do {
        int base_x, base_y;
        std::tie(base_x,base_y) = q.front();
        q.pop();
    
        if (board[base_y][base_x].region[0] != NO_REGION) 
            continue;

        board[base_y][base_x].region[0] = color;

        int min_x = 0,
            max_x = W-1;

        for (int x=base_x+1;x<=max_x;++x) {
            if (!board[base_y][x].is_piece) {
                board[base_y][x].region[0] = color;
            }
            else {
                board[base_y][x].region[Left] = color;
                max_x = x-1;
                break;
            }
        }

        for (int x=base_x-1;x>=min_x;--x) {
            if (!board[base_y][x].is_piece) {
                board[base_y][x].region[0] = color;
            }
            else {
                board[base_y][x].region[Right] = color;
                min_x = x+1;
                break;
            }
        }

        int upper_y = base_y-1;
        if (upper_y>=0) {
            bool last_emplaced = false;
            for (int x=min_x;x<=max_x;++x) {
                auto& cell = board[upper_y][x];
                if (cell.is_piece) {
                    cell.region[Down] = color;
                    last_emplaced = false;
                }
                else {
                    if (cell.region[0] == NO_REGION) {
                        if (!last_emplaced) {
                            q.emplace(x,upper_y);
                            // don't enqueue consecutive cells
                            last_emplaced = true;
                        }
                    }
                    else
                        last_emplaced = false;
                }
            }
        }

        int lower_y = base_y+1;
        if (lower_y<H) {
            bool last_emplaced = false;
            for (int x=min_x;x<=max_x;++x) {
                auto& cell = board[lower_y][x];
                if (cell.is_piece) {
                    cell.region[Up] = color;
                    last_emplaced = false;
                }
                else {
                    if (cell.region[0] == NO_REGION) {
                        if (!last_emplaced) {
                            q.emplace(x,lower_y);
                            // don't enqueue consecutive cells
                            last_emplaced = true;
                        }
                    }
                    else
                        last_emplaced = false;
                }
            }
        }
    }
    while (!q.empty());
    //std::cerr << "Region " << color << " has " << count << " cells\n";
}

int solve_at(int x, int y, Direction dir, int target_x, int target_y, int used, int limit) {
    
    if (used>limit || !coords_valid(x,y) || visited[y][x])
        return INF;
    
    if (y==target_y && x==target_x)
        return used;
    if (board[y][x].is_piece)
        return INF;

    visited[y][x] = true;
    //std::cerr << ">"<<x<<","<<y<<"\n";
    int nx = x + dir_delta[dir].first,
        ny = y + dir_delta[dir].second;
    
    int best = solve_at(nx,ny,dir,target_x,target_y,used,limit);

    if (used < limit) {
        std::array<Direction,2> alt;
        if (dir==Up || dir==Down) {
            alt[0] = Left;
            alt[1] = Right;
        }
        else {
            alt[0] = Up;
            alt[1] = Down;
        }
    
        for (int i=0;i<2;++i) {
            Direction d = alt[i];
            nx = x + dir_delta[d].first;
            ny = y + dir_delta[d].second;
            best = std::min(solve_at(nx,ny,d,target_x,target_y,used+1,limit),best);
            if (best <= used+1)
                break;
        }
    }

    visited[y][x] = false;
    return best;

}

int solve(int x1,int y1,int x2,int y2) {

    int x_dist = x1>x2? x1-x2 : x2-x1,
        y_dist = y1>y2? y1-y2 : y2-y1;

    if (x_dist+y_dist == 1) {
        // adjacent
        return 1;
    }

    const Item& a = board[y1][x1],
                b = board[y2][x2];
    //std::cerr << "a:" << x1<<","<<y1<<" b:"<<x2<<","<<y2<<"\n";
    std::vector<RegionId> regions = shared_regions(a,b);

    if (regions.empty())
        return -1;
 
    for (int limit=1;limit<LIMIT;++limit) {

        for (int y=0;y<H;++y) {
            visited[y].reset();
        }

        int best_num = INF;

        // let's try to solve for each region
        for (RegionId reg : regions) {
            for (int dir = 0; dir < MAX_ADJ_REGIONS; ++dir) {
                if (a.region[dir] == reg) {
                    int xpos = x1 + dir_delta[dir].first,
                        ypos = y1 + dir_delta[dir].second;
                    if (coords_valid(xpos,ypos)) {
                        int result = solve_at(xpos,ypos,static_cast<Direction>(dir),x2,y2,1,limit);
                        if (result < best_num) {
                            best_num = result;
                            if (best_num <= 2)
                                return best_num;
                        }
                    }
                }
            }
        }

        if (best_num != INF)
            return best_num;
    }
    return -1;
}

int main() {
    helper::BufferedStdout output;
    helper::LineReader<> reader;
    std::string line;
       
    for (int ncase=0;reader.get_line(line);++ncase) {
        std::array<int,2> dims = helper::num_array<int,2>(line);
        W = dims[0] + 2,
        H = dims[1] + 2;
        
        if (dims[0]==0)
            break;

        for (int y=1;y<=dims[1];++y) {
            board[y][0].is_piece = false;
            board[y][0].region[0] = NO_REGION;
            reader.get_line(line);
            for (int x=1;x<=dims[0];++x) {
                if ((board[y][x].is_piece = line[x-1]=='X')) {
                    board[y][x].region.fill(NO_REGION);
                }
                else {
                    board[y][x].region[0] = NO_REGION;
                }
            }
            board[y][dims[0]+1].is_piece = false;
            board[y][dims[0]+1].region[0] = NO_REGION;
        }

        for (int x=0;x<W;++x) {
            board[0][x].is_piece = false;
            board[0][x].region[0] = NO_REGION;
            board[H-1][x].is_piece = false;
            board[H-1][x].region[0] = NO_REGION;
        }
        
        RegionId rid = NO_REGION;
        for (int y=0;y<H;++y) {
            for (int x=0;x<W;++x) {
                if (!board[y][x].is_piece && board[y][x].region[0]==NO_REGION) {
                   flood_fill(x,y,++rid);
                }
            }
        }
        

        output.append("Board #").append(ncase+1).append(":\n");

        for (int npair=1;;++npair) {
            reader.get_line(line);
            std::array<int,4> req = helper::num_array<int,4>(line);
            if (req[0] == 0) {
                break;
            }

            output.append("Pair ").append(npair).append(": ");

            int ans = solve(req[0],req[1],req[2],req[3]);
            if (ans>=0) 
                output.append(ans).append(" segments.\n");
            else
                output.append("impossible.\n");
        }
        output.endl();
    }
    //output.endl();
}

