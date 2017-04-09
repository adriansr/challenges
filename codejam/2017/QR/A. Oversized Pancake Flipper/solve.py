
T = int(raw_input())

INF=999999999
S = 0
K = 0
visited = {}

def flip(state, pos):
    s = list(state)
    for i in range(pos, pos+K):
        s[i] = not s[i]
    return s

def solve(state):
    s = len(state)
    if all(state):
        return 0
    best = INF
    key = ''.join(map(lambda x: '+' if x else '-', state))
    if key in visited:
        return visited[key]
    visited[key] = best
    for i in range(0,S-K+1):
        best = min(best, 1+solve(flip(state,i)))
    visited[key] = best
    return best

for case_num in range(1, T+1):
    (base, ns) = raw_input().split(' ')
    S = len(base)
    K = int(ns)
    visited = {}
    flips = solve(map(lambda x: x == '+', list(base)))
    print 'Case #{0}: {1}'.format(case_num, flips if (flips<INF) else 'IMPOSSIBLE')
