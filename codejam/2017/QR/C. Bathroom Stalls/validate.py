import bisect

def split(v):
    s = v - 1 if v > 0 else 0
    minv = s / 2
    return (minv, s - minv)

def binary_str(num):
    if num < 2:
        return [ num != 0 ]
    r = []
    while num > 0:
        r.append( bool(num & 1) )
        num = num / 2
    return r

def solve(n, k):
    bits = binary_str(k)
    bits.pop()
    cur = n
    while len(bits)>0:
        direction = bits.pop()
        (a, b) = split(cur)
        cur = b if not direction else a
    return split(cur)

def brute(n, k):
    avail = [n]
    for i in range(k):
        seg = avail.pop() - 1
        minv = seg / 2
        maxv = seg - minv
        bisect.insort_left(avail, minv)
        bisect.insort_left(avail, maxv)
    return (minv, maxv)

for n in range(1, 20):
    for k in range(1, n+1):
        (a, b) = brute(n, k)
        (c, d) = solve(n, k)
#       print 'n={0} k={1} : ({2},{3}) = {4}'.format(n,k,a,b,a+b+1)
        if (a != c or b != d):
            print 'Failed for n=',n,'k=',k
            print 'brute: ',a,b
            print 'solve: ',c,d

# X_____X
# X__1__X
# X2_1__X
# X2_13_X
