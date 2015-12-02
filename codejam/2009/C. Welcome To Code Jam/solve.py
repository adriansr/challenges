import sys

target = 'welcome to code jam'

def solve(line):
    counters = [0] * len(target)
    for c in line:
        for i in [ i for i,x in enumerate(target) if x == c ]:
            if i>0:
                counters[i] += counters[i-1]
            else:
                counters[i] += 1

    return counters[-1]

f = open(sys.argv[1],'r')
N = int(f.readline())

for i in xrange(N):
    result = solve( f.readline().strip() )
    print 'Case #%d: %04d' % ( (i+1) , result % 10000 )



