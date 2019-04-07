# This is a simple solution that will only work for the
# visible test suite where F = log2 N.

import sys


def solve(N):
    num_queries = N - 1
    num_queries = num_queries.bit_length()

    queries = []
    for q in range(num_queries):
        queries.append(['0'] * N)
        for i in range(N):
            if i & (1 << q):
                queries[q][i] = '1'
    resp = []
    for q in queries:
        print ''.join(q)
        sys.stdout.flush()
        msg = raw_input()
        if msg == '-1':
            return
        resp.append(msg)

    nums = [0] * len(resp[0])
    for i in range(num_queries):
        for j in range(len(resp[i])):
            if resp[i][j] == '1':
                nums[j] = nums[j] | (1 << i)
    good = set(nums)
    bad = []
    for i in range(N):
        if i not in good:
            bad.append(str(i))

    print ' '.join(bad)
    sys.stdout.flush()
    raw_input()

T = int(raw_input())

for case_num in range(1, T+1):
    (N, B, F) = map(int, raw_input().split(' '))
    solve(N)
