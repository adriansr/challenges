
T = int(raw_input())

def flip(state, pos, k):
    for i in range(pos, pos+k):
        state[i] = not state[i]

def solve(state, k):
    s = len(state)
    num_flips = 0
    for i in range(0, s-k+1):
        if not state[i]:
            flip(state, i, k)
            num_flips += 1
    return num_flips if all(state) else -1

for case_num in range(1, T+1):
    (base, ns) = raw_input().split(' ')
    flips = solve(map(lambda x: x == '+', list(base)), int(ns))
    print 'Case #{0}: {1}'.format(case_num, flips if (flips>=0) else 'IMPOSSIBLE')
