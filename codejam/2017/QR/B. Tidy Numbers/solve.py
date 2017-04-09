
def solved(num, limit):
    not_greater = num <= limit
    if not_greater:
        snum = str(num)
        for i in range(1,len(snum)):
            if snum[i] < snum[i-1]:
                return False
    return not_greater

def solve(limit):
    num = limit
    while not solved(num, limit):
        snum = list(str(num))
        for pos in range(1, len(snum)):
            if snum[pos] < snum[pos-1]:
                break
        else:
             assert('num > limit somehow')
        substract = True
        while (substract and pos>=0):
            snum[pos] = chr(ord(snum[pos]) - 1)
            if snum[pos]<'0':
                pos -= 1
            else:
                substract = False
        for n in range(pos+1, len(snum)):
            snum[n] = '9'
        num = int(''.join(snum))
    return num

T = int(input())
for test_num in range(1,T+1):
    result = solve(int(input()))
    print 'Case #{0}: {1}'.format(test_num, result)
