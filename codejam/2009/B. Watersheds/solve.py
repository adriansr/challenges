import sys

class DrainageMap:
    def __init__(self,H,W,m):
        self.H = H
        self.W = W
        self.label = 'a'
        # input matrix (altitude)
        self.elev_map = m #[[ 0 for x in xrange(W)] for y in xrange(H)]
        # output matrix (label)
        self.label_map = [[ ' ' for x in xrange(W)] for y in xrange(H)]
    
    def solve(self):

        for y in xrange(H):
            for x in xrange(W):
                if self.label_map[y][x] == ' ':
                    self.solve_at( (y,x) )

        return self.label_map
    
    def elevation_at(self,pos):
        return self.elev_map[pos[0]][pos[1]]

    def flows(self,pos):
        # flows to min
        # flows North, West, East, South
        [y,x] = pos
        lowest_elev = elev = self.elevation_at(pos)
        lowest_pos = pos
    
        north = (y-1,x)
        if north[0]>=0:
            north_elev = self.elevation_at(north)
            if north_elev < lowest_elev:
                lowest_elev = north_elev
                lowest_pos = north

        west = (y,x-1)
        if west[1]>=0:
            west_elev = self.elevation_at(west)
            if west_elev < lowest_elev:
                lowest_elev = west_elev
                lowest_pos = west

        east = (y,x+1)
        if east[1]<self.W:
            east_elev = self.elevation_at(east)
            if east_elev < lowest_elev:
                lowest_elev = east_elev
                lowest_pos = east

        south = (y+1,x)
        if south[0]<self.H:
            south_elev = self.elevation_at(south)
            if south_elev < lowest_elev:
                lowest_elev = south_elev
                lowest_pos = south

        if lowest_elev < elev:
            return lowest_pos

        # it's a sink!
        return None

    def solve_at(self,pos):
        # positions we are traversing until a basin is found
        locations = []
        label = ' '
        while pos is not None:
            l = self.label_map[pos[0]][pos[1]]
            if l != ' ':
                label = l
                break
            locations.append(pos)
            pos = self.flows(pos)
        
        
        if label == ' ':
            label = self.label
            self.label = chr( 1 + ord(self.label))

        for pos in locations:
            self.label_map[pos[0]][pos[1]] = label

f = open(sys.argv[1],'r')
T = int(f.readline())

for i in xrange(T):
    [H,W] = map(int,f.readline().split())
    
    m = []
    for h in xrange(H):
        row = map(int,f.readline().split())
        assert len(row) == W
        m.append(row)

    P = DrainageMap(H,W,m)

    print 'Case #%d:' % (i+1)
    for line in P.solve():
        print ' '.join(line)
    
