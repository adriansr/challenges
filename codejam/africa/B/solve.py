import sys

def solve(items):
    for i in xrange(len(items)):
        yield items[-1 -i]

f = open(sys.argv[1],'r')
N = int(f.readline())

for i in xrange(N):
    #C = int(f.readline())
    #I = int(f.readline())
    items = f.readline().split()
    result = solve(items)
    print 'Case #%d:' % ((i+1)),
    for elem in result:
        print elem,
    print
