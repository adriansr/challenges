import sys
import numpy as np

def reduce(A,B):
    d = { key: value for value,key in enumerate(sorted(set(A+B))) }
    for i in xrange(len(A)):
        A[i] = d[A[i]]
        B[i] = d[B[i]]
    return A,B

def solve(A,B):
    (A,B) = reduce(A,B)
    N = 1 + max(A)
    M = 1 + max(B)
    isecs = np.zeros( (N,M) , dtype=int )
    count = 0
    for i in xrange(len(A)):
        [a,b] = [A[i],B[i]]

        count += isecs[a,b]
        
        if a>0 and b<=M:
            isecs[:a,(b+1):] += 1
        if a<=N and b>0:
            isecs[(a+1):,:b] += 1
        
    return count

f = open(sys.argv[1],'r')
T = int(f.readline())

for i in xrange(T):
    
    N = int(f.readline())
    A = []
    B = []
    for n in xrange(N):
        t = tuple(map(int,f.readline().split()))
        A.append(t[0])
        B.append(t[1])

    result = solve(A,B)
    print 'Case #%d: %d' % ( (i+1) , result )



