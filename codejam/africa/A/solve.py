import sys

def solve(C,I,items):
    for i in xrange(I-1):
        for j in xrange(i+1,I):
            if items[i] + items[j] == C:
                return (i,j)

f = open(sys.argv[1],'r')
N = int(f.readline())

for i in xrange(N):
    C = int(f.readline())
    I = int(f.readline())
    items = map(int,f.readline().split())
    result = solve(C,I,items)
    print 'Case #%d: %d %d' % ((i+1),result[0]+1,result[1]+1)
