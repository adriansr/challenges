

def solve(target):
    a = ''.join([x if x != '4' else '3' for x in target])
    b = ''.join(['0' if x != '4' else '1' for x in target])
    while len(b) > 0 and b[0] == '0':
        b = b[1:]
    if len(b) == 0:
        b = '0'
    return a, b

T = int(raw_input())

for case_num in range(1, T+1):
    target = raw_input()
    (a, b) = solve(target)
    print 'Case #{0}: {1} {2}'.format(case_num, a, b)
