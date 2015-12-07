def solve_instance(m,k): # 4 3
    n = 2 * k # n = 6
    #if 1+(m%n) <= k:
    #    return False
    
    pos = 0

    alive = [' '] + [ 'g' ] * k + [ 'b' ] * k
    while len(alive) > k+1:
        pos += m
        if pos > n:
            pos = pos % n
            if pos == 0:
                pos = n
        if alive[pos] == ' ':
            raise 'fuck this shit'
        if alive[pos] == 'g':
            return False
        del alive[pos]
        n -= 1
        pos -= 1
        if pos <= 0:
            pos += n

    return True
            

def solve(k):
    m = k+1 #4
    while True:
        if solve_instance(m,k):
            return m
        m += 1

#print solve(3)

for i in xrange(1,14):
    v = str(solve(i))
    print '\t{"'+v+'",'+str(len(v))+'}, /*',i,'*/'

