import bisect

def brute(n, k):
    avail = [n]
    for i in range(k):
        seg = avail.pop() - 1
        minv = seg / 2
        maxv = seg - minv
        bisect.insort_left(avail, minv)
        bisect.insort_left(avail, maxv)
    return (minv, maxv)

T = int(raw_input())
for test_num in range(1,T+1):
    line = raw_input()
    (n, k) = map(int, line.split(' '))
    (minv, maxv) = brute(n, k)
    print 'Case #{0}: {1} {2}'.format(test_num, maxv, minv)
