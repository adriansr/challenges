import sys

class Solver:
    def __init__(self):
        self.root = {}
        self.count_created = 0
    
    def add(self,path):
        count = 0
        dirent = self.root
        for name in path.split('/')[1:]:
            if name not in dirent:
                count += 1
                dirent[name] = {}
            dirent = dirent[name]
        return count
                
    def add_existing(self,path):
        self.add(path)

    def add_missing(self,path):
        self.count_created += self.add(path)

    def solve(self):
        return self.count_created

f = open(sys.argv[1],'r')
T = int(f.readline())

for i in xrange(T):
    
    s = Solver()

    [N, M] = map(int,f.readline().split())
    for n in xrange(N):
        s.add_existing(f.readline().strip())
    for m in xrange(M):
        s.add_missing(f.readline().strip())

    result = s.solve()
    print 'Case #%d: %d' % ( (i+1) , result )



