
def gcd(a, b):
    if b > a:
        a, b = b, a
    while b != 0:
        t = b
        b = a % b
        a = t
    return a


def solve(nums):
    L = len(nums)
    primes = [0] * (L + 1)

    # get pos of first non-repeated pair of numbers
    pos, numA = 0, nums[0]
    while nums[pos+1] == numA:
        pos += 1
    numB = nums[pos+1]

    first_prime = gcd(numA, numB)
    primes[pos+1] = first_prime

    for i in range(pos, -1, -1):
        primes[i] = nums[i] / primes[i+1]

    for i in range(pos+2, L+1):
        primes[i] = nums[i-1] / primes[i-1]

    # validate
    #for i in range(L-1):
    #    assert nums[i] == primes[i] * primes[i+1], "failed for {0} in {1} / {2}".format(i, primes, nums)

    r = list(set(primes))
    r.sort()

    toLetters = {r[i]: chr(ord('A') + i) for i in range(len(r))}
    return ''.join(map(lambda x: toLetters[x], primes))


T = int(raw_input())

for case_num in range(1, T+1):
    (N, L) = map(int, raw_input().split(' '))
    msg = map(int, raw_input().split(' '))
    assert len(msg) == L
    print 'Case #{0}: {1}'.format(case_num, solve(msg))
