import sys

def solve(A,B):
    A.sort()
    B.sort()
    B.reverse()
    r = 0
    for i in xrange(len(A)):
        r += A[i] * B[i]
    return r

f = open(sys.argv[1],'r')
T = int(f.readline())

for i in xrange(T):
    N = int(f.readline())
    A = map(int,f.readline().split())
    B = map(int,f.readline().split())
    if len(A) != N or len(B) != N:
        raise('wrong vector length')

    result = solve(A,B)
    print 'Case #%d: %d' % ((i+1),result)
