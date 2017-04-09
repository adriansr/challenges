
def split(v):
    s = v - 1
    minv = s / 2
    return (minv, s - minv)

def count_bits(n):
    for i in range(64):
        pos = 1 << i
        n &= ~pos
        if n == 0:
            return i
    raise "Number too large"

def solve(n, k):
    layer = count_bits(k)
    prev_nodes = (1 << layer) - 1
    this_nodes = prev_nodes + 1
    seats_rem = n - prev_nodes
    val = seats_rem / this_nodes
    rem = seats_rem % this_nodes
    return split(val + 1) if (k <= prev_nodes + rem) else split(val)
    
T = int(raw_input())
for test_num in range(1,T+1):
    line = raw_input()
    (n, k) = map(int, line.split(' '))
    (minv, maxv) = solve(n, k)
    print 'Case #{0}: {1} {2}'.format(test_num, maxv, minv)

